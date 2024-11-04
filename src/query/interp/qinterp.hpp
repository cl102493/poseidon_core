#ifndef qinterp_hpp_
#define qinterp_hpp_

#include "query_batch.hpp"
#include "graph_db.hpp"
#include "query_ctx.hpp"

class qinterp {
public:
  /**
   * Constructor
   */
  qinterp() = default;

  /**
   * Destructor
   */

  ~qinterp() = default;

private:
  /**
   * Starts the given query within the context of a new transaction.
   */
  void execute(query_ctx& ctx, query_batch& qbatch);
};

#endif
