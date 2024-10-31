
#ifndef nodes_hpp_
#define nodes_hpp_

#include <atomic>
#include <map>
#include <vector>
#include <utility>
#include <any>

#include "vec.hpp"
#include "defs.hpp"
#include "exceptions.hpp"
#include "properties.hpp"
#include "transaction.hpp"
#include "txn_data.hpp"

struct node;
using dirty_node = dirty_object<node>;
using dirty_node_ptr = std::unique_ptr<dirty_node>;

/**
 * node represents a vertex in a graph with a non-unique label and a set of
 * properties, which can be part of a relationship. Both, properties and
 * relationships are stored in separate lists. The actual label is stored in a
 * dictionary, only the code value is stored as part of the node.
 */
struct node : public txn<dirty_node_ptr> {
  friend class graph_db;
  
  template <template <typename I> typename V> friend class node_list;

  using id_t =
      offset_t; // typedef for node identifier (used as offset in node list)
private:
  id_t id_;
public:
  offset_t from_rship_list; // index in relationship list of first relationship
                            // where this node acts as from node
  offset_t to_rship_list;   // index of relationship list of first relationship
                            // where this node acts as to node
  offset_t property_list;   // index in property list
  dcode_t node_label;       // dictionary code for node label

  /**
   * Default constructor.
   */
  node() = default;

  /**
   * Copy constructor.
   */
  node(const node &) = delete;

  node(node &&n)
      : txn(n), id_(n.id_), from_rship_list(n.from_rship_list),
        to_rship_list(n.to_rship_list), property_list(n.property_list),
        node_label(n.node_label) {}

  /**
   * Constructor for creating a node with the given label code.
   */
  node(dcode_t label)
      : id_(UNKNOWN), from_rship_list(UNKNOWN), to_rship_list(UNKNOWN),
        property_list(UNKNOWN), node_label(label) {}

    /**
   * Copy assignment operator. This implementation is needed because of atomic
   * xid_t.
   */
  node &operator=(const node &n) {
    txn::operator=(n);
    node_label = n.node_label;
    from_rship_list = n.from_rship_list;
    to_rship_list = n.to_rship_list;
    property_list = n.property_list;
    id_ = n.id_;

    return *this;
  }
	
  /**
   * Move assignment operator. This implementation is needed because of atomic  xid_t.
   * Move resources from source node.
   */
  node &operator=( node &&n) {
    txn::operator=(std::move(n));
    node_label = n.node_label;
    from_rship_list = n.from_rship_list;
    to_rship_list = n.to_rship_list;
    property_list = n.property_list;
    id_ = n.id_;

    return *this;
  }

  /**
   * Returns the node identifier.
   */
  id_t id() const { return id_; }

  std::size_t _offset() const {
    return (uint64_t)((uint8_t *)&id_) - (uint64_t)((uint8_t *)this);
  }
};

/**
 * A class providing complete information for a node, i.e. string values for
 * labels and properties are available instead of only the dictionary codes.
 */
struct node_description {
  node::id_t id;           // the node identifier
  std::string label;       // the label (type)
  properties_t properties; // the list of properties

  /**
   * Return a string representation of the node_description object
   */
  std::string to_string() const;

  /**
   * Return true if a property with the given name exists
   */
  bool has_property(const std::string& pname) const;

  bool operator==(const node_description& other) const;
};

/**
 * A class for storing all nodes of a graph. It supports adding and removing
 * nodes as well as getting a node via its node_id.
 */
template <template <typename I> typename T> class node_list {

  struct init_node_task {};

public:
  using vec = T<node>;
  using range_iterator = typename T<node>::range_iter;

  /**
   * Constructor
   */
  template <typename... Args>
  node_list(Args &&...args) : nodes_(std::forward<Args>(args)...) {}

  node_list(const node_list &) = delete;

  /**
   * Destructor
   */
  ~node_list() = default;

  /**
   * Add a new node to the list and return its identifier. The node is inserted
   * into the first available slot, i.e. to reuse space of deleted records.
   * If owner != 0 then the newly created node is locked by this owner
   * transaction.
   */
  node::id_t add(node &&n, xid_t owner = 0) {
    if (nodes_.is_full())
    nodes_.resize(1);

  auto id = nodes_.first_available();
  assert(id != UNKNOWN);
  n.id_ = id;
  if (owner != 0) {
    /// spdlog::info("lock node #{} by {}", id, owner);
    n.lock(owner);
  }
  nodes_.store_at(id, std::move(n));
  return id;  
  }

    /**
   * Append a new node to the list and return its identifier. In contrast to add
   * the node is appended at the end of the list without checking for available
   * slots. If owner == 0 then the newly created node is locked by this owner
   * transaction. If a callback is given this function is called before the slot 
   * is reserved (used for undo logging).
   */
  node::id_t append(node &&n, xid_t owner = 0, std::function<void(offset_t)> callback = nullptr) {
    auto p = nodes_.append(std::move(n), callback);
  p.second->id_ = p.first;
  if (owner != 0) {
    /// spdlog::info("lock node #{} by {}", p.first, owner);
    p.second->lock(owner);
  }

  return p.first;  
  }

  /**
   * Get a node via its identifier.
   */
  node &get(node::id_t id) {
   if (nodes_.capacity() <= id) {
    spdlog::warn("unknown node_id {}", id);
    throw unknown_id();
   }
  auto &n = nodes_.at(id);
  return n;   
  }

  /**
   * Returns the underlying vector of the node list.
   */
  auto &as_vec() { return nodes_; }


  /**
   * Returns the number of occupied chunks of the underlying chunked_vec.
   */
  std::size_t num_chunks() const { return nodes_.num_chunks(); }

private:
  T<node> nodes_; // the actual list of nodes
};

#endif
