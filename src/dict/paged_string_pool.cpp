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
#include <vector>

paged_string_pool::paged_string_pool(bufferpool& bp, uint64_t fid) : 
    bpool_(bp), file_id_(fid), file_mask_(fid << 60) {
    spdlog::debug("paged_string_pool created");
    npages_ = bpool_.get_file(file_id_)->num_pages();
    if (npages_ == 0) {
        // we have a new file
        bpool_.allocate_page(file_id_);
        npages_ = 1;
    }

    std::string filename = "/Users/lei/Desktop/poseidon/poseidon_core/demo/testdb/symbol_table.bin";
    if (std::filesystem::exists(filename)) {
        load_symbol_table(filename);
    }

}

void paged_string_pool::initialize_compression(const std::vector<std::string>& samples) {
    std::cout << "初始化压缩" << std::endl;
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
        bpool_.flush_page(pg.second | file_mask_, false);
        std::vector<std::string> samples;
        scan([&samples](const char* s, dcode_t) {
            if (samples.size() < SAMPLE_SIZE) {
                samples.emplace_back(s);
            }
        });
        initialize_compression(samples);
        save_symbol_table("/Users/lei/Desktop/poseidon/poseidon_core/demo/testdb/symbol_table.bin");
        // return
        compression.printSymbolTable();
    }
    return pos;

}

dcode_t paged_string_pool::add_compressed(const std::string& str) {
    auto compressed = compression.compressString(str);

    //std::cout << "compression" <<std::endl;
    
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

// bool paged_string_pool::scan_memory_pages(std::function<void(const char *s, dcode_t c)> cb) {
//     uint32_t npage = 0u; // number of pages processed

//     for (paged_file::page_id pid = 1; pid <= npages_; ++pid) {
//         auto full_pid = pid | file_mask_;
//         auto pg = bpool_.get_page_if_loaded(full_pid);
        
//         if (pg) {  // 页面在内存中
//             auto data = &(pg->payload[0]);
//             auto lastp = sizeof(uint32_t);
//             auto spos = (pid - 1) * PAGE_SIZE + sizeof(uint32_t);
//             auto ppos = spos;

//             for (auto p = sizeof(uint32_t); p < PAGE_SIZE; p++, spos++) {
//                 if (data[p] == '\0' && data[lastp] != '\0') {
//                     cb((const char *)&data[lastp], ppos);
//                     lastp = p + 1;
//                     ppos = spos + 1;
//                 }
//             }
//             npage++;
//         }
//     }

//     if (npage == 0) {
//         spdlog::info("No pages found in memory.");
//         return false;
//     }
    
//     spdlog::info("Scanned {} pages in memory.", npage);
//     return true;
// }

const char* paged_string_pool::extract(dcode_t pos) const {
    paged_file::page_id pid = pos / PAGE_SIZE + 1;
    auto pg = bpool_.fetch_page(pid | file_mask_);
    auto page_offset = pos % PAGE_SIZE;

    if (!compression_ready_ || pos < COMPRESSION_THRESHOLD) {
        return (const char*)&(pg->payload[page_offset]);
    }

    const unsigned char* compressed_start = &(pg->payload[page_offset]);
    size_t compressed_size = 0;
    size_t remaining_in_page = PAGE_SIZE - page_offset;

    // 查找压缩字符串的结束位置（'\0'）
    const unsigned char* end = static_cast<const unsigned char*>(memchr(compressed_start, '\0', remaining_in_page));
    
    if (end) {
        compressed_size = end - compressed_start;
    } else {
        // 如果在当前页面没有找到结束符，需要查找下一个页面
        compressed_size = remaining_in_page;
        pid++;
        pg = bpool_.fetch_page(pid | file_mask_);
        end = static_cast<const unsigned char*>(memchr(pg->payload, '\0', PAGE_SIZE));
        if (end) {
            compressed_size += end - pg->payload;
        } else {
            throw std::runtime_error("Compressed string end not found");
        }
    }

    // 解压缩
    static thread_local std::vector<unsigned char> compressed_buffer;
    compressed_buffer.assign(compressed_start, compressed_start + compressed_size);
    
    static thread_local std::string decompressed;
    decompressed = compression.decompressString(compressed_buffer);

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

bool paged_string_pool::save_symbol_table(const std::string& filename) {
    if (!compression_ready_) {
        spdlog::error("Compression not initialized. Cannot save symbol table.");
        return false;
    }

    std::cout << "compression.exportSymbolTable()" << std::endl;

    std::vector<uint8_t> buffer = compression.exportSymbolTable();
    if (buffer.empty()) {
        spdlog::error("Failed to export symbol table");
        return false;
    }
  
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        spdlog::error("Failed to open file for writing: {}", filename);
        return false;
    }

    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    if (!file.good()) {
        spdlog::error("Failed to write symbol table to file: {}", filename);
        return false;
    }

    std::cout << "filename:" << filename << std::endl;

    spdlog::info("Symbol table saved to file: {}", filename);
    return true;
}

bool paged_string_pool::load_symbol_table(const std::string& filename) {
    std::cout << "filename:" << filename << std::endl;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        spdlog::error("Failed to open file for reading: {}", filename);
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        spdlog::error("Failed to read file: {}", filename);
        return false;
    }

    if (!compression.importSymbolTable(buffer)) {
        spdlog::error("Failed to import symbol table from file: {}", filename);
        return false;
    }

    compression_ready_ = true;
    spdlog::info("Symbol table loaded from file: {}", filename);
    return true;
}

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
