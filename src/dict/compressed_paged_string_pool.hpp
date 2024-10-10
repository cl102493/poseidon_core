#ifndef compressed_paged_string_pool_hpp
#define compressed_paged_string_pool_hpp

#include "defs.hpp"
#include "fsst.h"
#include "paged_string_pool.hpp"

class compressed_paged_string_pool : public paged_string_pool {
public:
  compressed_paged_string_pool(bufferpool &bp, uint64_t fid);
  ~compressed_paged_string_pool();

  dcode_t add(const std::string &str) override;
  const char *extract(dcode_t pos) const override;
  bool equal(dcode_t pos, const std::string &s) const override;

  double get_compression_ratio() const { return compression_ratio; }

  void save_fsst_encoder(const std::string &path) const;
  void load_fsst_encoder(const std::string &path);

private:
  void initialize_fsst();
  std::string compress_string(const std::string &input) const;
  std::string decompress_string(const std::string &compressed) const;
  // void try_initialize_fsst();
  dcode_t set_compression_flag(dcode_t code) const {
    return code | COMPRESSION_FLAG;
  }

  bool is_compressed(dcode_t code) const {
    return (code & COMPRESSION_FLAG) != 0;
  }

  dcode_t clear_compression_flag(dcode_t code) const {
    return code & ~COMPRESSION_FLAG;
  }

  pool_fsst_encoder_t *fsst_encoder;
  mutable pool_fsst_decoder_t fsst_decoder;

  std::atomic<double> compression_ratio;
  std::atomic<bool> fsst_initialized;
  std::vector<std::string> sample_strings;
  static const size_t SAMPLE_THRESHOLD = 100;
  static const dcode_t COMPRESSION_FLAG =1U << 31; // Use the highest bit as a compression flag
};

#endif
