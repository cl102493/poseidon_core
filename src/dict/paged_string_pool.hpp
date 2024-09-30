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
#ifndef paged_string_pool_hpp
#define paged_string_pool_hpp

#include <string>
#include "defs.hpp"
#include "bufferpool.hpp"
#include "compression.hpp"

class paged_string_pool {
public:
    paged_string_pool(bufferpool& bp, uint64_t fid);
    ~paged_string_pool() = default;
    
    const char* extract(dcode_t pos) const;
    bool equal(dcode_t pos, const std::string& s) const;
    
    dcode_t add(const std::string& str);
    
    bool scan(std::function<void(const char *s, dcode_t c)> cb);
    //bool scan_memory_pages(std::function<void(const char *s, dcode_t c)> cb);
    void print() const;

    // 新增方法
    bool is_compression_ready() const { return compression_ready_; }
    
private:
    bufferpool& bpool_;
    uint64_t file_id_, file_mask_;
    uint64_t npages_;
    Compression compression;
    bool compression_ready_ = false;

    void initialize_compression(const std::vector<std::string>& samples);
    dcode_t add_uncompressed(const std::string& str);
    dcode_t add_compressed(const std::string& str);
    std::vector<std::string> sample_strings() const;

    size_t string_count_ = 0;
    static constexpr size_t COMPRESSION_THRESHOLD = 100;
    static constexpr size_t SAMPLE_SIZE = 1000;

};

#endif /* paged_string_pool_hpp */
