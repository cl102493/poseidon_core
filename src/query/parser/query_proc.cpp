#include "query_proc.hpp"
#include "query_planner.hpp"

#include "antlr4-runtime.h"
#include "antlr4_generated/poseidonBaseVisitor.h"
#include "antlr4_generated/poseidonLexer.h"
#include "antlr4_generated/poseidonParser.h"

query_proc::query_proc(query_ctx &ctx) : qctx_(ctx) {}


//适用于快速语法检查
bool query_proc::parse_(const std::string &query) {
  antlr4::ANTLRInputStream input(query);
  poseidonLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  poseidonParser parser(&tokens);
  parser.setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());

  try {
    poseidonParser::QueryContext *tree = parser.query();
    return tree != nullptr;
  } catch (std::exception &exc) {
    return false;
  }
}

qresult_iterator query_proc::execute_query(mode m, const std::string &qstr,
                                           bool print_plan) {
  auto qplan = prepare_query(qstr);

  result_set result;
  qplan.append_collect(result);
  prepare_plan(qplan);

  if (m == Interpret) {
    interp_query(qplan);
  } else {
    // TODO: compile & execute query
    // qplan.print_plan();
    compile_query(qplan);
  }
  if (print_plan)
    qplan.print_plan();

  return qresult_iterator(std::move(result));
}

std::size_t query_proc::execute_and_output_query(mode m,
                                                 const std::string &qstr,
                                                 bool print_plan) {}

// 用于实际的查询执行准备
query_batch query_proc::prepare_query(const std::string &query) {
    antlr4::ANTLRInputStream input(query);
    poseidonLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    poseidonParser parser(&tokens);
    LexerErrorListener lexerErrorListener;
    ParserErrorListener parserErrorListener;

    lexer.removeErrorListeners();
    lexer.addErrorListener(&lexerErrorListener);

    parser.removeParseListeners();
    parser.removeErrorListeners();
    parser.addErrorListener(&parserErrorListener);

    poseidonParser::QueryContext* tree = parser.query();

    query_planner visitor(qctx_);
    if (udf_lib_ && udf_lib_->is_loaded())
        visitor.add_udf_library(udf_lib_);
    
    visitor.visitQuery(tree);
    return visitor.get_query_plan();
}

void query_proc::interp_query(query_batch &plan) {}
void query_proc::compile_query(query_batch &plan) {}

void query_proc::abort_query() {}

void query_proc::abort_transaction() {}

bool query_proc::load_library(const std::string &lib_path) {}
