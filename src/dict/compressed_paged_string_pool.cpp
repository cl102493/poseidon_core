#include "compressed_paged_string_pool.hpp"
#include "spdlog/spdlog.h"
#include <iostream>

compressed_paged_string_pool::compressed_paged_string_pool(bufferpool &bp,
                                                           uint64_t fid)
    : paged_string_pool(bp, fid), fsst_encoder(nullptr),
      fsst_initialized(false) {}

compressed_paged_string_pool::~compressed_paged_string_pool() {
  if (fsst_encoder) {
    duckdb_fsst_destroy(fsst_encoder);
  }
}

void compressed_paged_string_pool::initialize_fsst() {
  std::vector<size_t> sample_lengths;
  std::vector<unsigned char *> sample_string_ptrs;

  for (const auto &s : sample_strings) {
    sample_lengths.push_back(s.length());
    sample_string_ptrs.push_back(
        reinterpret_cast<unsigned char *>(const_cast<char *>(s.c_str())));
  }

  fsst_encoder =
      duckdb_fsst_create(sample_strings.size(), sample_lengths.data(),
                         sample_string_ptrs.data(), 1);
  fsst_decoder = duckdb_fsst_decoder(fsst_encoder);
  fsst_initialized.store(true);

  // sample_strings.clear();
}

// void compressed_paged_string_pool::try_initialize_fsst() {
//   if (!fsst_initialized.load() &&
//       uncompressed_count.load() >= SAMPLE_THRESHOLD) {
//     //std::cout << "Calling initialize_fsst()" << std::endl;
//     initialize_fsst();
//   }
// }

dcode_t compressed_paged_string_pool::add(const std::string &str) {
  // try_initialize_fsst();
  if (!fsst_initialized.load()) {
    sample_strings.push_back(str);
    dcode_t code = paged_string_pool::add(str);
    if (sample_strings.size() >= SAMPLE_THRESHOLD) {
      initialize_fsst();
    }
    return code;
  }

  std::string compressed = compress_string(str);
  dcode_t code = paged_string_pool::add(compressed);
  return set_compression_flag(code);
}

std::string
compressed_paged_string_pool::compress_string(const std::string &input) const {
  std::vector<unsigned char> output(input.length() * 2 +
                                    7); // Worst case scenario
  size_t compressed_length;
  unsigned char *compressed_ptr;

  size_t input_length = input.length();
  unsigned char *input_ptr =
      reinterpret_cast<unsigned char *>(const_cast<char *>(input.c_str()));

  duckdb_fsst_compress(fsst_encoder, 1, &input_length, &input_ptr,
                       output.size(), output.data(), &compressed_length,
                       &compressed_ptr);

  return std::string(output.data(), output.data() + compressed_length);
}

std::string compressed_paged_string_pool::decompress_string(
    const std::string &compressed) const {
  std::vector<unsigned char> output(compressed.length() *
                                    4); // Estimate decompressed size
  size_t decompressed_length = duckdb_fsst_decompress(
      &fsst_decoder, compressed.length(),
      reinterpret_cast<unsigned char *>(const_cast<char *>(compressed.c_str())),
      output.size(), output.data());

  return std::string(reinterpret_cast<char *>(output.data()),
                     decompressed_length);
}

const char *compressed_paged_string_pool::extract(dcode_t pos) const {
  if (!is_compressed(pos)) {
    return paged_string_pool::extract(pos);
  }
  pos = clear_compression_flag(pos);
  const char *stored = paged_string_pool::extract(pos);
  static thread_local std::string decompressed;
  decompressed = decompress_string(stored);
  return decompressed.c_str();

  //   const char *stored = paged_string_pool::extract(pos);
  //   if (!fsst_initialized.load()) {
  //     return stored;
  //   }
  //   static thread_local std::string decompressed;
  //   decompressed = decompress_string(stored);
  //   return decompressed.c_str();
}

bool compressed_paged_string_pool::equal(dcode_t pos,
                                         const std::string &s) const {
  const char *stored = paged_string_pool::extract(pos);
  if (!fsst_initialized.load()) {
    return strcmp(stored, s.c_str()) == 0;
  }
  return decompress_string(stored) == s;
}

void compressed_paged_string_pool::save_fsst_encoder(
    const std::string &path) const {
  if (fsst_initialized.load()) {
    std::ofstream outfile(path, std::ios::binary);
    if (outfile.is_open()) {
      std::vector<uint8_t> buffer(FSST_MAXHEADER);
      uint32_t len = duckdb_fsst_export(fsst_encoder, buffer.data());
      buffer.resize(len);
      outfile.write(reinterpret_cast<const char *>(buffer.data()), len);
      outfile.close();
    }
  }
}

void compressed_paged_string_pool::load_fsst_encoder(const std::string &path) {
  std::ifstream infile(path, std::ios::binary);
  if (!infile.is_open()) {
    spdlog::warn("FSST encoder file not found: {}. Continuing without loading.",
                 path);
    return;
  }
  infile.seekg(0, std::ios::end);
  std::streampos fileSize = infile.tellg();
  infile.seekg(0, std::ios::beg);

  std::vector<uint8_t> buffer(fileSize);
  infile.read(reinterpret_cast<char *>(buffer.data()), fileSize);

  if (infile.gcount() > 0) {
    duckdb_fsst_decoder_t new_decoder;
    uint32_t consumed =
        duckdb_fsst_import(&new_decoder, const_cast<uint8_t *>(buffer.data()));
    fsst_decoder = new_decoder;
  }

  infile.close();
  fsst_initialized.store(true);
}
