#include <filesystem>
#include <fstream>
#include <iostream>

#include "defs.hpp"
#include "graph_db.hpp"
#include "graph_pool.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/program_options.hpp>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"


enum cmd_mode {
  undefined_mode,
  import_mode,
  script_mode,
  shell_mode
};

using namespace boost::program_options;

graph_pool_ptr pool;
graph_db_ptr graph;

/**
 * Import data from the given list of CSV files. The list contains
 * not only the files names but also nodes/relationships as well as
 * the labels.
 */
bool import_csv_files(graph_db_ptr &gdb, std::string import_path, const std::vector<std::string> &files,
                      char delimiter, std::string format, bool strict) {
  graph_db::mapping_t id_mapping;
  std::cout<< "import_csv_files" << std::endl;

  for (auto s : files) {
    if (s.find("nodes:") != std::string::npos) {
      std::vector<std::string> result;
      boost::split(result, s, boost::is_any_of(":"));

      if (result.size() != 3) {
        std::cerr << "ERROR: unknown import option for nodes." << std::endl;
        return false;
      }

      std::size_t num = 0;
      auto file_name = import_path;
      if (!file_name.empty()) file_name += "/";
      file_name += result[2];

      auto start = std::chrono::steady_clock::now();
      if (format == "n4j") {
        num = gdb->import_typed_n4j_nodes_from_csv(result[1], file_name,
                                                   delimiter, id_mapping);
      }
      else {
        num = strict
          ? gdb->import_typed_nodes_from_csv(result[1], file_name, delimiter, id_mapping)
          : gdb->import_nodes_from_csv(result[1], file_name, delimiter, id_mapping);
      }
      auto end = std::chrono::steady_clock::now();

      auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
      spdlog::info("{} '{}' nodes imported in {} msecs ({} items/s)", num, result[1], time, (int)((double)num/time * 1000.0));
    }
    else if (s.find("relationships:") != std::string::npos) {
      std::vector<std::string> result;
      boost::split(result, s, boost::is_any_of(":"));

      if (format == "n4j") {
        if (result.size() < 2 || result.size() > 3) {
          std::cerr << "ERROR: unknown import option for relationships."
                    << std::endl;
          return false;
        }
      }
      else if (result.size() != 3) {
          std::cerr << "ERROR: unknown import option for relationships."
                    << std::endl;
          return false;
      }

      std::size_t num = 0;
      auto file_name = import_path;
      if (!file_name.empty()) file_name += "/";

      auto start = std::chrono::steady_clock::now();
      if (format == "n4j") {
        file_name += result.back();
        auto rship_type = result.size() == 3 ? result[1] : "";
        num = gdb->import_typed_n4j_relationships_from_csv(file_name, delimiter, id_mapping, rship_type);
      }
      else {
        file_name += result[2];
        num = strict
         ? gdb->import_typed_relationships_from_csv(file_name, delimiter, id_mapping)
         : gdb->import_relationships_from_csv(file_name, delimiter, id_mapping);
      }
      auto end = std::chrono::steady_clock::now();

      auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
      if (result.size() == 3) 
        spdlog::info("{} '{}' relationships imported in {} msecs ({} items/s)", num, result[1], time, (int)((double)num/time * 1000.0));
      else
        spdlog::info("{} relationships imported in {} msecs ({} items/s)", num, time, (int)((double)num/time * 1000.0));
    }
    else {
      std::cerr << "ERROR: unknown import (nodes or relationships expected)."
                << std::endl;
      return false;
    }
  }
  gdb->flush();
  return true;
}


std::string check_config_files(const std::string &fname) {
  std::filesystem::path cwd_config_file(fname);
  if (std::filesystem::exists(cwd_config_file))
    return cwd_config_file.string();

  std::string full_name = getenv("HOME") + std::string("/") + fname;
  std::filesystem::path home_config_file(full_name);
  if (std::filesystem::exists(home_config_file))
    return home_config_file.string();

  return "";
}

void show_help() {
  std::cout << "Available commands:\n"
            << "\thelp                             " << "show this help" << "\n"
            << "\tstring s                         " << "display the dictionary code of the string s" << "\n"
            << "\tcode c                           " << "display the string of the dictionary code c" << "\n"
            << "\tload <library>                   " << "load the given shared library" << "\n"
            << "\tstats                            " << "print database statistics" << "\n"
            << "\tsync                             " << "ensure that all pages are written to disk" << "\n"
            << "\tcreate index <label> <property>  " << "create an index for the given label/property" << "\n"
            << "\tdrop index <label> <property>    " << "delete the index for the given label/property" << "\n"
            << "\t@file                            " << "execute the query stored in the given file" << "\n"
            << "\texplain <query-expr>             " << "execute the given query and print the plan" << "\n"
            << "\t<query-expr>                     " << "execute the given query" << "\n"
            << "\tprint node|rship <id>            " << "print the raw data of the node/relationship with given id" << std::endl;
}

int main(int argc, char *argv[]) {
  spdlog::info("hello poseidon");
  std::string db_name, pool_path, query_file, import_path, dot_file, qmode_str, format = "ldbc";
  std::size_t bp_size = 0;
  std::vector<std::string> import_files;
  bool start_shell = false;
  char delim_character = ',';
   bool strict = false;
  cmd_mode mode = undefined_mode;

  auto console = spdlog::stdout_color_mt("poseidon");
  spdlog::set_default_logger(console);
  //spdlog::set_level(spdlog::level::debug); // 设置日志级别为debug
  spdlog::info("Starting poseidon cli, Version {}", POSEIDON_VERSION);

  try {
    options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Help")
        ("verbose,v", bool_switch()->default_value(false), "Verbose - show debug output")
        ("db,d", value<std::string>(&db_name)->required(), "Database name (required)")
        ("pool,p", value<std::string>(&pool_path)->required(), "Path to the PMem/file pool")
        ("buffersize,b", value<std::size_t>(&bp_size), "Size of the bufferpool (in pages)")
        ("output,o", value<std::string>(&dot_file), "Dump the graph to the given file (in DOT format)")
        ("strict", bool_switch()->default_value(true), "Strict mode - assumes that all columns contain values of the same type")
        ("delimiter", value<char>(&delim_character)->default_value('|'), "Character delimiter")
        ("format,f", value<std::string>(&format), "CSV format: n4j | gtpc | ldbc")
        ("import-path", value<std::string>(&import_path), "Directory containing import files")
        ("import", value<std::vector<std::string>>()->composing(),
        "Import files in CSV format (either nodes:<node type>:<filename> or "
        "relationships:<rship type>:<filename>");
        //("query,q", value<std::string>(&query_file), "Execute the query from the given file")
        //("shell,s", bool_switch()->default_value(false), "Start the interactive shell")
        //("qmode", value<std::string>(&qmode_str), "Query compile mode: llvm | interp (default) | adapt");


    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    auto config_name = check_config_files("poseidon.ini");
    // std::cout << "config_name: " << config_name << std::endl;

    if (!config_name.empty()) {
      spdlog::info("loading config from '{}'", config_name);
      std::ifstream ifs(config_name);
      store(parse_config_file(ifs, desc), vm);
    }

    if (vm.count("help")) {
      std::cout << "Poseidon Graph Database Version " << POSEIDON_VERSION
                << "\n"
                << desc << '\n';
      std::cout << "help" << std::endl;
      return -1;
    }

    notify(vm);

    // for (const auto& pair : vm) {
    //     std::cout << pair.first << ": ";
    //     if (pair.second.value().type() == typeid(std::string)) {
    //         std::cout << pair.second.as<std::string>() << std::endl;
    //     } else if (pair.second.value().type() == typeid(bool)) {
    //         std::cout << std::boolalpha << pair.second.as<bool>() <<
    //         std::endl;
    //     } else {
    //         std::cout << "Unknown type" << std::endl;
    //     }
    // }

    if (vm.count("import_path"))
      import_path = vm["import_path"].as<std::string>();

    if (vm.count("import")) {
      import_files = vm["import"].as<std::vector<std::string>>();
    }

    if (vm.count("pool"))
      pool_path = vm["pool"].as<std::string>();

    if (vm.count("buffersize"))
      bp_size = vm["buffersize"].as<std::size_t>();

    if (vm.count("delimiter")) {
      delim_character = vm["delimiter"].as<char>();
    }

    if (vm.count("format")) {
      format = vm["format"].as<std::string>();
    }

    if (format != "n4j" && format != "qtpc" && format != "ldbc") {
      std::cerr << "ERROR: choose format -- n4j or --qtpc or -- ldbc.\n";
      return -1;
    }

     if (vm.count("verbose")){
    if (vm["verbose"].as<bool>()) {
        spdlog::set_level(spdlog::level::debug);
    }
  }
  } catch (const error &ex) {
    std::cerr << ex.what() << '\n';
    return -1;
  }

  if (access(pool_path.c_str(), F_OK) != 0) {
    spdlog::info("creat poolset {}", pool_path);
    pool = graph_pool::create(pool_path);
    graph = pool->create_graph(db_name, bp_size);
  } else {
    spdlog::info("open poolset {}", pool_path);
    pool = graph_pool::open(pool_path, true);
    graph = pool->open_graph(db_name, bp_size);
  }

  if (!import_files.empty()) {
    spdlog::info("--------------  importing files --------------");
    import_csv_files(graph, import_path, import_files, delim_character, format, strict);
    graph->print_stats();
  }

  if (!dot_file.empty()) {
    // nothing
  }
  

  return 0;
}
