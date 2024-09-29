/*
 * Copyright (C) 2019-2022 DBIS Group - TU Ilmenau, All Rights Reserved.
 *
 * This file is part of the Poseidon package.
 *
 * Poseidon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Poseidon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Poseidon. If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <cstdlib>
#include "paged_string_pool.hpp"
#include "spdlog/spdlog.h"

paged_string_pool::paged_string_pool(bufferpool& bp, uint64_t fid) : 
    bpool_(bp), file_id_(fid), file_mask_(fid << 60) {
    spdlog::debug("paged_string_pool created");
    npages_ = bpool_.get_file(file_id_)->num_pages();
    if (npages_ == 0) {
        // we have a new file
        bpool_.allocate_page(file_id_);
        npages_ = 1;
    }
}

void paged_string_pool::initialize_compression(const std::vector<std::string>& samples) {
    if (compression_ready_) return;
    
    if (compression.initializeEncoder(samples)) {
        compression_ready_ = true;
    }
}


dcode_t paged_string_pool::add(const std::string& str) {
    if (!compression_ready_) {
        return add_uncompressed(str);
    }
    return add_compressed(str);
}

dcode_t paged_string_pool::add_uncompressed(const std::string& str) {
     auto pg = bpool_.last_valid_page(file_id_);
    uint32_t last_pos = 0; 
    
    memcpy(&last_pos, &(pg.first->payload[0]), sizeof(uint32_t));
    if (last_pos == 0) {
        last_pos += sizeof(uint32_t);
    }

    auto page_pos = last_pos - (npages_ - 1) * PAGE_SIZE; // the position on the page
    if (page_pos + str.length() + 1 >= PAGE_SIZE) {
        pg = bpool_.allocate_page(file_id_);
        npages_++;
        last_pos = (npages_ - 1) * PAGE_SIZE + sizeof(uint32_t);
        page_pos = sizeof(uint32_t);
    }
    
    dcode_t pos = last_pos;
    memcpy(&(pg.first->payload[page_pos]), str.c_str(), str.length());
    last_pos += str.length() + 1;
    page_pos += str.length() + 1;
    pg.first->payload[page_pos - 1] = '\0';

    memcpy(&(pg.first->payload[0]), &last_pos, sizeof(uint32_t));
    // mark the current page as dirty
    bpool_.mark_dirty(pg.second | file_mask_);

    string_count_++;
    if (!compression_ready_ && string_count_ >= COMPRESSION_THRESHOLD) {
        std::vector<std::string> samples;
        scan([&samples](const char* s, dcode_t) {
            if (samples.size() < SAMPLE_SIZE) {
                samples.emplace_back(s);
            }
        });
        initialize_compression(samples);
    }

    return pos;

}

dcode_t paged_string_pool::add_compressed(const std::string& str) {
    auto compressed = compression.compressString(str);

    std::cout << "compression" <<std::endl;
    
    auto pg = bpool_.last_valid_page(file_id_);
    uint32_t last_pos = 0;
    
    memcpy(&last_pos, &(pg.first->payload[0]), sizeof(uint32_t));
    if (last_pos == 0) {
        last_pos += sizeof(uint32_t);
    }

    auto page_pos = last_pos - (npages_ - 1) * PAGE_SIZE;
    if (page_pos + compressed.size() >= PAGE_SIZE) {
        pg = bpool_.allocate_page(file_id_);
        npages_++;
        last_pos = (npages_ - 1) * PAGE_SIZE + sizeof(uint32_t);
        page_pos = sizeof(uint32_t);
    }
    
    dcode_t pos = last_pos;
    memcpy(&(pg.first->payload[page_pos]), compressed.data(), compressed.size());
    last_pos += compressed.size() + 1;
    page_pos += compressed.size() + 1;
    pg.first->payload[page_pos - 1] = '\0';

     memcpy(&(pg.first->payload[0]), &last_pos, sizeof(uint32_t));
    bpool_.mark_dirty(pg.second | file_mask_);

    return pos;
}

bool paged_string_pool::scan(std::function<void(const char *s, dcode_t c)> cb) { // 2
    uint32_t npage = 0u; // number of page processed

    bpool_.scan_file(file_id_, [&](auto pg) {
        auto data = &(pg->payload[0]);
        auto lastp = sizeof(uint32_t);
        auto spos = npage * PAGE_SIZE + sizeof(uint32_t);
        auto ppos = spos;

        for (auto p = sizeof(uint32_t); p < PAGE_SIZE; p++, spos++) {
            if (data[p] == '\0' && data[lastp] != '\0') {
                cb((const char *)&data[lastp], ppos);
                lastp = p + 1;
                ppos = spos + 1;
            }
        }
        npage++;
    });  
    if (npage != npages_) {
        spdlog::info("ERROR: string dictionary corrupted - only {} of {} pages processed.", npage, npages_);
        return false; 
    }
    return true;
}

const char* paged_string_pool::extract(dcode_t pos) const {
    paged_file::page_id pid = pos / PAGE_SIZE + 1;
    auto pg = bpool_.fetch_page(pid | file_mask_);
    auto page_offset = pos % PAGE_SIZE;

    if (!compression_ready_ || pos < COMPRESSION_THRESHOLD) {
        return (const char*)&(pg->payload[page_offset]);
    }

    // 读取压缩数据
    uint32_t next_pos;
    memcpy(&next_pos, &(pg->payload[0]), sizeof(uint32_t));
    size_t compressed_size = next_pos - pos;

    std::vector<unsigned char> compressed_data(compressed_size);
    memcpy(compressed_data.data(), &(pg->payload[page_offset]), compressed_size);

    // 解压缩
    static thread_local std::string decompressed;
    decompressed = compression.decompressString(compressed_data);

    return decompressed.c_str();
}

// const char *paged_string_pool::extract(dcode_t pos) const {
//     paged_file::page_id pid = pos / PAGE_SIZE + 1;
//     auto pg = bpool_.fetch_page(pid | file_mask_);
//     auto page_offset = pos % PAGE_SIZE;
//     return (const char *) &(pg->payload[page_offset]);
// }

bool paged_string_pool::equal(dcode_t pos, const std::string& s) const {
    paged_file::page_id pid = pos / PAGE_SIZE + 1;
    auto pg = bpool_.fetch_page(pid | file_mask_);
    auto page_offset = pos % PAGE_SIZE;
    auto i = 0u;
    for (; i < s.length() && i + page_offset < PAGE_SIZE; i++)
        if (pg->payload[page_offset + i] != s.at(i))
            return false;
    return pg->payload[page_offset + i] == '\0';
}

// dcode_t paged_string_pool::add(const std::string& str) {
//     // std::cout << "paged_string_pool::add()" << str <<  "\n";
//     auto pg = bpool_.last_valid_page(file_id_);
//     uint32_t last_pos = 0; // the total position over all pages = dict code of str
    
//     memcpy(&last_pos, &(pg.first->payload[0]), sizeof(uint32_t));
//     if (last_pos == 0) {
//         //std::cout << "last_pos in paged_string_pool::add():" << last_pos << "\n";
//         last_pos += sizeof(uint32_t);  // sizeof(uint32_t) = 4
//     }

//     auto page_pos = last_pos - (npages_ - 1) * PAGE_SIZE; // the position on the page
//     if (page_pos + str.length() + 1 >= PAGE_SIZE) {
//         // we need a new page
//         pg = bpool_.allocate_page(file_id_);
//         npages_++;
//         last_pos = (npages_ - 1) * PAGE_SIZE + sizeof(uint32_t);
//         page_pos = sizeof(uint32_t);
//     }
//     dcode_t pos = last_pos/* + (npages_ - 1) * PAGE_SIZE*/;
//     memcpy(&(pg.first->payload[page_pos]), str.c_str(), str.length());
//     // spdlog::info("add string '{}'/{} at page_pos={}(last_pos={}) - {}", str, pos, page_pos, last_pos, page_pos + str.length());
//     last_pos += str.length() + 1;
//     page_pos += str.length() + 1; 
//     pg.first->payload[page_pos - 1] = '\0';
//     // we store last_pos always in the first few bytes of the last page
//     memcpy(&(pg.first->payload[0]), &last_pos, sizeof(uint32_t));
//     // mark the current page as dirty
//     bpool_.mark_dirty(pg.second | file_mask_);
//     return pos;
// }

void paged_string_pool::print() const {
    //std::cout << std::string(pool_, last_) << std::endl;
    spdlog::debug("enter_pool_print");

    if (npages_ == 0) {
        spdlog::info("pages = 0");
    } else {
       bpool_.scan_file(file_id_, [&](auto pg) {
           uint32_t npage = 0u; // number of page processed
           auto data = &(pg->payload[0]);
           std::cout << static_cast<int>(data[0])  << std::endl;

           auto lastp = sizeof(uint32_t);
           auto spos = npage * PAGE_SIZE + sizeof(uint32_t);
           auto ppos = spos;

           for (auto p = sizeof(uint32_t); p < PAGE_SIZE; p++, spos++) {
               if (data[p] == '\0' && data[lastp] != '\0') {
                   //spdlog::info("pool_print");
                   std::cout << (const char *)&data[lastp] << "," << ppos << "\n";
                   lastp = p + 1;
                   ppos = spos + 1;
               }
           }
           npage++;
       });
    }
}
