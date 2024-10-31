#include "graph_db.hpp"
#include "spdlog/spdlog.h"
#include <iostream>

#define UNDO_CB nullptr

void graph_db::destroy(graph_db_ptr gp) {
  std::filesystem::path path_obj(gp->database_name_);
  if (std::filesystem::exists(path_obj))
    std::filesystem::remove_all(path_obj);
}

void graph_db::prepare_files(const std::string &pool_path,
                             const std::string &pfx) {
  spdlog::debug("graph_db: prepare files {} / {}", pool_path, pfx);
  std::filesystem::path path_obj(pool_path);
  path_obj /= pfx;
  // check if path exists and is of a regular file
  if (!std::filesystem::exists(path_obj))
    std::filesystem::create_directory(path_obj);

  std::string prefix = path_obj.string() + "/";

  spdlog::debug("graph_db: prepare files in '{}'", prefix);

  node_file_ = std::make_shared<paged_file>();
  node_file_->open(prefix + "nodes.db", NODE_FILE_ID);
  bpool_.register_file(NODE_FILE_ID, node_file_);

  rship_file_ = std::make_shared<paged_file>();
  rship_file_->open(prefix + "rships.db", RSHIP_FILE_ID);
  bpool_.register_file(RSHIP_FILE_ID, rship_file_);

  nprops_file_ = std::make_shared<paged_file>();
  nprops_file_->open(prefix + "nprops.db", NPROPS_FILE_ID);
  bpool_.register_file(NPROPS_FILE_ID, nprops_file_);

  rprops_file_ = std::make_shared<paged_file>();
  rprops_file_->open(prefix + "rprops.db", RPROPS_FILE_ID);
  bpool_.register_file(RPROPS_FILE_ID, rprops_file_);

  dict_ = p_make_ptr<dict>(bpool_, prefix);
}

graph_db::graph_db(const std::string &db_name, const std::string &pool_path,
                   std::size_t bpool_size)
    : database_name_(db_name),
      bpool_(bpool_size == 0 ? DEFAULT_BUFFER_SIZE : bpool_size) {
  pool_path_ = pool_path;
  prepare_files(pool_path, db_name);
  nodes_ = p_make_ptr<node_list<buffered_vec>>(bpool_, NODE_FILE_ID);
  rships_ = p_make_ptr<relationship_list<buffered_vec>>(bpool_, RSHIP_FILE_ID);
  node_properties_ = p_make_ptr<property_list<buffered_vec>>(bpool_, NPROPS_FILE_ID);
  rship_properties_ = p_make_ptr<property_list<buffered_vec>>(bpool_, RPROPS_FILE_ID);
}

graph_db::~graph_db() {}

void graph_db::flush() { bpool_.flush_all(); }


 /**
   * Print some stats about memory usage.
   */
void graph_db::print_stats() {
  
}
