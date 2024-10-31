#include "graph_pool.hpp"
#include "spdlog/spdlog.h"
#include "exceptions.hpp"
#include <filesystem>
#include <iostream>

graph_pool_ptr graph_pool::create(const std::string &path,
                                  unsigned long long pool_size) {
  auto self = std::make_unique<graph_pool>();
  self->path_ = path;
  std::filesystem::path path_obj(path);
  if (!std::filesystem::exists(path_obj)) {
    std::filesystem::create_directory(path_obj);
  }
  return self;
}

graph_pool_ptr graph_pool::open(const std::string &path, bool init) {
  auto self = std::make_unique<graph_pool>();
  self->path_ = path;
  std::filesystem::path path_obj(path);
  if (!std::filesystem::exists(path_obj)) {
    spdlog::info("FATAL: graph_pool '{}' doesn't exist.", path);
    abort();
  }
  return self;
}
void graph_pool::destroy(graph_pool_ptr &p) {
  // for (auto& gp: p->graphs_) {
  //     graph_db::destrory(gp.second);
  // }
  // std::filesystem::path path_obj(p->path_);
  // std::filesystem::remove_all(path_obj);
}
graph_pool::graph_pool() {   
}

graph_pool::~graph_pool() {}

graph_db_ptr graph_pool::create_graph(const std::string &name,
                                      std::size_t bpool_size) {
  std::cout << "bpool_size: " << bpool_size << std::endl;
  auto gptr = p_make_ptr<graph_db>(name, path_, bpool_size);
  graphs_.insert({name, gptr});
  return gptr;
}

graph_db_ptr graph_pool::open_graph(const std::string &name,
                                    std::size_t bpool_size) {
  // TODO: check whether graph directory exists
  std::filesystem::path path_obj(path_);
  path_obj /= name;
  if (!std::filesystem::exists(path_obj)) {
    spdlog::info("FATAL: graph '{}' doesn't exist in pool '{}'.", name, path_);
    throw unknown_db();
  }
  auto gptr = p_make_ptr<graph_db>(name, path_, bpool_size);
  graphs_.insert({name, gptr});
  return gptr;
}

void graph_pool::drop_graph(const std::string &name) {
    auto iter = graphs_.find(name);
    if (iter == graphs_.end()) throw unknown_db();
    std::filesystem::path path_obj(name);
    std::filesystem::remove_all(path_obj);
    graphs_.erase(iter);
}

void graph_pool::close() {

}
