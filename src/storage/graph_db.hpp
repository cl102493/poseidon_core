
#ifndef graph_db_
#define graph_db_

#include <any>
#include <map>
#include <mutex>
#include <string>

#include "dict.hpp"
#include "exceptions.hpp"
#include "nodes.hpp"
#include "properties.hpp"
#include "relationships.hpp"
#include "vec.hpp"
#include "transaction.hpp"
//#include "btree.hpp"
//#include "index_map.hpp"
//#include "walog.hpp"
#include "gc.hpp"
#include "robin_hood.h"
#include "bufferpool.hpp"

//#include "analytics.hpp"

class graph_db {
public:
  
  using mapping_t = robin_hood::unordered_map<std::string, node::id_t>;

   static void destroy(p_ptr<graph_db> gp);

  /**
   * Constructor for a new empty graph database.
   */
  graph_db(const std::string &db_name = "", const std::string &pool_path = "", std::size_t bpool_size = DEFAULT_BUFFER_SIZE);


  /**
   * Destructor.
   */
  ~graph_db();

  /* -------------- transaction management -------------- */

  /* ---------------- graph construction ---------------- */

  /**
   * Add a new node to the graph with the given label (type) and the
   * set of properties (key-value pairs). The method returns the node identifier
   * which can be used to create a relationship from or to this node.
   */
  node::id_t add_node(const std::string &label, const properties_t &props,
                      bool append_only = false);

  /**
   * Add a new node to the graph with the given label (type) and the
   * set of properties (key-value pairs) without transactional support.
   * The method returns the node identifier which can be used to create a
   * relationship from or to this node.
   */
  node::id_t import_node(const std::string &label, const properties_t &props);

  node::id_t import_typed_node(dcode_t label, const std::vector<dcode_t> &keys,
                              const std::vector<p_item::p_typecode>& typelist, 
                              const std::vector<std::any>& values);

  node::id_t import_typed_node(dcode_t label, const std::vector<dcode_t> &keys,
                              const std::vector<p_item::p_typecode>& typelist,
							  const std::vector<std::string>& values,dict_ptr &dict);

  /**
   * Add a new relationship to the graph that connects from_node and to_node.
   * This relationship has initialized with the given label and properties.
   */
  relationship::id_t add_relationship(node::id_t from_node, node::id_t to_node,
                                      const std::string &label,
                                      const properties_t &props,
                                      bool append_only = false);

  /**
   * Add a new relationship to the graph that connects from_node and to_node
   * without transactional support. This relationship has initialized with the
   * given label and properties.
   */
  relationship::id_t import_relationship(node::id_t from_node,
                                         node::id_t to_node,
                                         const std::string &label,
                                         const properties_t &props);

  relationship::id_t import_typed_relationship(node::id_t from_node,
                                         node::id_t to_node,
                                         dcode_t label, 
                                         const std::vector<dcode_t> &keys,
                                         const std::vector<p_item::p_typecode>& typelist, 
					  const std::vector<std::string>& values,dict_ptr &dict);


  relationship::id_t import_typed_relationship(node::id_t from_node,
                                         node::id_t to_node,
                                         dcode_t label,
                                         const std::vector<dcode_t> &keys,
                                         const std::vector<p_item::p_typecode>& typelist,
                                         const std::vector<std::any>& values);


  /* --------------- node/relationship information --------------- */

  /* --------------- graph updates --------------- */

  /* ---------------- CSV data import ---------------- */
   /**
   * Read the list of nodes from the given CSV file. The file is in ldbc
   * format with the given delimiter.
   */
  std::size_t import_nodes_from_csv(const std::string &label,
                                    const std::string &filename, char delim,
                                    mapping_t &m, std::mutex *mtx = nullptr);

  std::size_t import_typed_nodes_from_csv(const std::string &label,
                                    const std::string &filename, char delim,
                                    mapping_t &m, std::mutex *mtx = nullptr);
  std::size_t import_typed_n4j_nodes_from_csv(const std::string &label,
                                    const std::string &filename, char delim,
                                    mapping_t &m);

  /**
   * Read the list of relationships from the given CSV file. The file is in
   * ldbc format with the given delimiter.
   */
  std::size_t import_relationships_from_csv(const std::string &filename,
                                            char delim, const mapping_t &m, std::mutex *mtx = nullptr);

  std::size_t import_typed_relationships_from_csv(const std::string &filename,
                                            char delim, const mapping_t &m, std::mutex *mtx = nullptr);

   std::size_t import_typed_n4j_relationships_from_csv(const std::string &filename,
                                            char delim, const mapping_t &m, const std::string& rship_type = "");

  /* ---------------- helper ---------------- */

   /**
   * Print some stats about memory usage.
   */
  void print_stats();

  /* ---------------- index management ---------------- */

  /* ---------------- Analytics support ---------------- */
  /**
   * Writes all modified pages from the bufferpool back to disk.
   */
  void flush();

  void close_files();

private:

  void prepare_files(const std::string &pool_path, const std::string &prefix);

  std::string database_name_; //
  bufferpool bpool_; //
  std::shared_ptr<paged_file> node_file_, rship_file_, nprops_file_, rprops_file_; //
  std::string pool_path_; //

  p_ptr<node_list<buffered_vec> > nodes_; // the list of all nodes of the graph
  p_ptr<relationship_list<buffered_vec> > rships_; // the list of all relationships of the graph
  p_ptr<property_list<buffered_vec> >
      node_properties_;   // the list of all properties of nodes 
  p_ptr<property_list<buffered_vec> >
      rship_properties_;   // the list of all properties of relationships

  p_ptr<dict> dict_; // the dictionary used for string compression



  /**
   * These member variables are volatile and have to be reinitialized
   * during startup.
   */
  std::mutex *m_;    // mutex for accessing active_tx_

};

using graph_db_ptr = p_ptr<graph_db>;

#endif
