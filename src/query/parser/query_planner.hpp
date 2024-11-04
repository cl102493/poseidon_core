#ifndef query_planner_hpp_
#define query_planner_hpp_

#include "antlr4_generated/poseidonBaseVisitor.h"
#include "antlr4_generated/poseidonLexer.h"
#include "antlr4_generated/poseidonParser.h"

class query_planner : public poseidonBaseVisitor {
public:
  query_planner(query_ctx &ctx) : qctx_(ctx) {}

private:
  query_ctx &qctx_;
};
#endif
