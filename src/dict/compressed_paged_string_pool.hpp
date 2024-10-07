#ifndef compressed_paged_string_pool_hpp
#define compressed_paged_string_pool_hpp

#include "fsst.h"
#include "defs.hpp"
#include "paged_string_pool.hpp"

class compressed_paged_string_pool : public paged_string_pool {
public:
  compressed_paged_string_pool(bufferpool &bp, uint64_t fid);
  ~compressed_paged_string_pool();

  virtual dcode_t add(const std::string &str);
  virtual const char *extract(dcode_t pos) const;
  virtual bool equal(dcode_t pos, const std::string &s) const;

  double get_compression_ratio() const { return compression_ratio; }

private:
  duckdb_fsst_encoder_t *fsst_encoder;
  mutable duckdb_fsst_decoder_t fsst_decoder;
  
  double compression_ratio;

  void initialize_fsst();
  std::string compress_string(const std::string &input);
  std::string decompress_string(const std::string &compressed) const;
};

#endif
