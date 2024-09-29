#include <vector>
#include <iostream>
#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP
#include <iomanip>
#include <cctype>

#include "fsst.h"

class Compression {
public:
    Compression() : encoder(nullptr) {}

    ~Compression() {
        if (encoder) {
            duckdb_fsst_destroy(encoder);
        }
    }

    bool initializeEncoder(const std::vector<std::string>& samples) {
        if (encoder) {
            duckdb_fsst_destroy(encoder);
        }

        std::vector<size_t> lengths;
        std::vector<unsigned char*> string_pointers;

        for (const auto& str : samples) {
            lengths.push_back(str.length());
            string_pointers.push_back((unsigned char*)str.c_str());
        }

        encoder = duckdb_fsst_create(samples.size(), lengths.data(), string_pointers.data(), 0);

        if (!encoder) {
            std::cerr << "Failed to create FSST encoder" << std::endl;
            return false;
        }

        decoder = duckdb_fsst_decoder(encoder);
        return true;
    }

    std::vector<unsigned char> compressString(const std::string& str) const{
        if (!encoder) {
            std::cerr << "Encoder not initialized. Call initializeEncoder() first." << std::endl;
            return {};
        }

        size_t max_compressed_size = str.length() * 2 + 7; // 保守估计
        std::vector<unsigned char> compressed(max_compressed_size);
        size_t compressed_length;
        unsigned char* compressed_ptr = compressed.data();
        size_t input_length = str.length();
        unsigned char* input_ptr = (unsigned char*)str.c_str();

        size_t compressed_count = duckdb_fsst_compress(
            encoder,
            1,
            &input_length,
            &input_ptr,
            max_compressed_size,
            compressed.data(),
            &compressed_length,
            &compressed_ptr
        );

        if (compressed_count == 0) {
            std::cerr << "Compression failed for string: " << str << std::endl;
            return {};
        }

        compressed.resize(compressed_length);
        return compressed;
    }

    std::string decompressString(const std::vector<unsigned char>& compressed) const{
        std::string decompressed;
        decompressed.resize(compressed.size() * 2); // 保守估计

        size_t decompressed_length = duckdb_fsst_decompress(
            &decoder,
            compressed.size(),
            const_cast<unsigned char*>(compressed.data()),
            decompressed.size(),
            (unsigned char*)decompressed.data()
        );

        decompressed.resize(decompressed_length);
        return decompressed;
    }

    void printSymbolTable() const {
    if (!encoder) {
        std::cerr << "Encoder not initialized. Call initializeEncoder() first." << std::endl;
        return;
    }

    std::cout << "FSST Symbol Table (Decimal):" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "Code | Length | Symbol" << std::endl;
    std::cout << "-----+--------+-------" << std::endl;

    for (int i = 0; i < 256; ++i) {  // FSST uses 1-byte codes, so max 256 symbols
        if (decoder.len[i] > 0) {
            std::cout << std::setw(4) << i << " | "
                      << std::setw(6) << static_cast<int>(decoder.len[i]) << " | ";
            
            // Print the symbol (up to 8 bytes)
            for (int j = 0; j < decoder.len[i] && j < 8; ++j) {
                unsigned char byte = (decoder.symbol[i] >> (j * 8)) & 0xFF;
                if (std::isprint(byte)) {
                    std::cout << byte;
                } else {
                    std::cout << std::setw(3) << std::setfill('0') 
                              << static_cast<int>(byte) << " ";
                }
            }
            std::cout << std::endl;
        }
    }
}

private:
    duckdb_fsst_encoder_t* encoder;
    mutable duckdb_fsst_decoder_t decoder;
};

#endif // COMPRESSION_HPP
