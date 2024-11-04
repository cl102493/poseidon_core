#ifndef query_ctx_hpp_
#define query_ctx_hpp_

#include "defs.hpp"
#include "graph_db.hpp"

class query_pipeline;

struct query_ctx {
    graph_db_ptr gdb_;

      /**
   * Constructors.
   */
    query_ctx() = default;
    query_ctx(query_ctx& ctx) : gdb_(ctx.gdb_) {}
    query_ctx(graph_db_ptr& gdb) : gdb_(gdb) {}

  /**
   * Destructors.
   */
    ~query_ctx();


  using node_consumer_func = std::function<void(node &)>;
  using rship_consumer_func = std::function<void(relationship &)>;
  /* -------------- transaction management -------------- */
  /**
   * Starts a new transation. This transation is associated with the 
   * current thread and stored there is thread_local property.
   */


  /* ---------------- dictionary access ---------------- */
  dict_ptr get_dictionary() { return gdb_->get_dictionary(); }

  dcode_t get_code(const std::string& s) { return gdb_->get_code(s); }

  const char* get_string(dcode_t c) { return gdb_->get_string(c);}
  
  /* ---------------- query support ---------------- */
};

struct scan_task {


};

struct scan_task_with_label {

};


#endif
