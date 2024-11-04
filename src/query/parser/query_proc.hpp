#ifndef query_proc_hpp_
#define query_proc_hpp_

#include <any>
#include <string>

#include <boost/dll/import.hpp>

#include "query_ctx.hpp"
#include "query_batch.hpp"

#include "qinterp.hpp"
#include "qresult_iterator.hpp"


/**
 * query_proc is the main entry class for the Poseidon query processor. It
 * encapsulates the parser, the query planner as well as the query compiler and
 * interpreter.
 */
class query_proc {
public:
    enum mode { Interpret, Compile, Adaptive };

    query_proc(query_ctx &ctx);
    ~query_proc() = default;

    bool parse_(const std::string &query);

    query_batch prepare_query(const std::string &query);

    qresult_iterator execute_query(mode m, const std::string &qstr,bool print_plan = false);

    std::size_t execute_and_output_query(mode m, const std::string &qstr, bool print_plan = false);

    void interp_query(query_batch &plan);
    void compile_query(query_batch &plan);

    void abort_query();

    void abort_transaction();

    bool load_library(const std::string &lib_path);

private:
    void prepare_plan(query_batch &qplan);
    query_ctx& qctx_;
    qinterp interp_;
    std::shared_ptr<boost::dll::shared_library> udf_lib_;
};
#endif
