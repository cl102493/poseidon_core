#include "compressed_paged_string_pool.hpp"

compressed_paged_string_pool::compressed_paged_string_pool(bufferpool &bp,
                                                           uint64_t fid)
    : paged_string_pool(bp, fid), fsst_encoder(nullptr),
      compression_ratio(1.0) {
  initialize_fsst();
}

compressed_paged_string_pool::~compressed_paged_string_pool() {
  if (fsst_encoder) {
    duckdb_fsst_destroy(fsst_encoder);
  }
}

void compressed_paged_string_pool::initialize_fsst() {
  std::vector<size_t> sample_lengths;
  std::vector<unsigned char *> sample_strings;
  const size_t sample_count = 1000; // Adjust as needed

  scan([&](const char *s, dcode_t c) {
    if (sample_strings.size() < sample_count) {
      size_t len = strlen(s);
      sample_lengths.push_back(len);
      sample_strings.push_back(
          reinterpret_cast<unsigned char *>(const_cast<char *>(s)));
    }
  });

  fsst_encoder = duckdb_fsst_create(
      sample_strings.size(), sample_lengths.data(), sample_strings.data(), 0);
  fsst_decoder = duckdb_fsst_decoder(fsst_encoder);
}

std::string
compressed_paged_string_pool::compress_string(const std::string &input) {
  std::vector<unsigned char> output(input.length() * 2); // Worst case scenario
  size_t compressed_length;
  unsigned char *compressed_ptr;

  size_t input_length = input.length();
  unsigned char *input_ptr =
      reinterpret_cast<unsigned char *>(const_cast<char *>(input.c_str()));

  duckdb_fsst_compress(fsst_encoder, 1, &input_length, &input_ptr,
                       output.size(), output.data(), &compressed_length,
                       &compressed_ptr);

  return std::string(reinterpret_cast<char *>(compressed_ptr),
                     compressed_length);
}

std::string compressed_paged_string_pool::decompress_string(const std::string& compressed) const {
    std::vector<unsigned char> output(compressed.length() * 4);  // Estimate decompressed size
    size_t decompressed_length = duckdb_fsst_decompress(
        &fsst_decoder,
        compressed.length(),
        reinterpret_cast<unsigned char*>(const_cast<char*>(compressed.c_str())),
        output.size(),
        output.data()
    );

    return std::string(reinterpret_cast<char*>(output.data()), decompressed_length);
}

dcode_t compressed_paged_string_pool::add(const std::string &str) {
  std::string compressed = compress_string(str);
  dcode_t code = paged_string_pool::add(compressed);

//   std::string compressed = compress_string(str);
//     return paged_string_pool::add(compressed);

  compression_ratio =
      (compression_ratio * (code - 1) +
       static_cast<double>(compressed.length()) / str.length()) /
      code;

  return code;
}

const char *compressed_paged_string_pool::extract(dcode_t pos) const {
  const char *compressed = paged_string_pool::extract(pos);
  static thread_local std::string decompressed;
  decompressed = decompress_string(compressed);
  return decompressed.c_str();
}

bool compressed_paged_string_pool::equal(dcode_t pos,
                                         const std::string &s) const {
  const char *compressed = paged_string_pool::extract(pos);
  return decompress_string(compressed) == s;
}
