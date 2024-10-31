#ifndef relationships_hpp_
#define relationships_hpp_

#include <vector>

#include "vec.hpp"
#include "defs.hpp"
#include "exceptions.hpp"
#include "nodes.hpp"
#include "transaction.hpp"
#include "txn_data.hpp"

struct relationship;
using dirty_rship = dirty_object<relationship>;
using dirty_rship_ptr = std::unique_ptr<dirty_rship>;

struct relationship : public txn<dirty_rship_ptr> {
  template <template <typename I> typename V> friend class relationship_list;

  using id_t = offset_t;

private:
  id_t id_;

public:
  offset_t src_node;       // from-node of the relationship (index in node list)
  offset_t dest_node;      // to-node of the relationship (index in node list)
  offset_t next_src_rship; // next relationship of the from-node (index in
                           // relationship list)
  offset_t next_dest_rship; // next relationship of the to-node (index in
                            // relationship list)
  offset_t property_list;   // index in property list
  dcode_t rship_label;      // dictionary code for relationship type

  /**
   * Default constructor.
   */
  relationship() = default;

  /**
   * Constructor for a new relationship with the given label code which connects
   * the given two nodes.
   */
  relationship(dcode_t rlabel, offset_t src, offset_t dest)
      : id_(UNKNOWN), src_node(src), dest_node(dest), next_src_rship(UNKNOWN),
        next_dest_rship(UNKNOWN), property_list(UNKNOWN), rship_label(rlabel) {}

  relationship::id_t id() const { return id_; }

  /**
   * Returns the node identifier of the to-node.
   */
  node::id_t to_node_id() const { return dest_node; }

  /**
   * Returns the node identifier of the from-node.
   */
  node::id_t from_node_id() const { return src_node; }
};

struct rship_description {
  relationship::id_t id;     // the relationship identifier
  node::id_t from_id, to_id; // identifiers of the source and destination node
  std::string label;         // the label (type)
  properties_t properties;   // the list of properties

  /**
   * Return a string representation of the node_description object.
   */
  std::string to_string() const;

  /**
   * Return true if a property with the given name exists.
   */
  bool has_property(const std::string &pname) const;

  bool operator==(const rship_description &other) const;
};

template <template <typename I> typename T> class relationship_list {

  struct init_rship_task {};

public:
  /**
   * Constructors.
   */
  template <typename... Args>
  relationship_list(Args&& ... args) : rships_(std::forward<Args>(args)...) {} 


  relationship_list(const relationship_list &) = delete;

  /**
   * Destructor.
   */
  ~relationship_list() = default;

  /**
   * Add a new relationship to the list and return its identifier. The
   * relationship is inserted into the first available slot, i.e. to reuse space
   * of deleted records.
   * If owner != 0 then the newly created relationship is locked by this owner
   * transaction. If a callback is given this function is called before the slot
   * is reserved (used for undo logging).
   */
  relationship::id_t add(relationship &&r, xid_t owner = 0) {
    if (rships_.is_full())
      rships_.resize(1);

    auto id = rships_.first_available();
    assert(id != UNKNOWN);
    r.id_ = id;
    if (owner != 0) {
      /// spdlog::info("lock relationship #{} by {}", id, owner);
      r.lock(owner);
    }
    rships_.store_at(id, std::move(r));
    return id;
  }

  /**
   * Append a new relationship to the list and return its identifier. In
   * contrast to add the relationship is appended at the end of the list without
   * checking for available slots.
   * If owner != 0 then the newly created relationship is locked by this owner
   * transaction. If a callback is given this function is called before the slot
   * is reserved (used for undo logging).
   */
  relationship::id_t append(relationship &&r, xid_t owner = 0,
                            std::function<void(offset_t)> callback = nullptr) {
    auto p = rships_.append(std::move(r), callback);
    p.second->id_ = p.first;
    if (owner != 0) {
      /// spdlog::info("lock relationship #{} by {}", p.first, owner);
      p.second->lock(owner);
    }
    return p.first;
  }

   /**
   * Get a relationship via its identifier.
   */
  relationship &get(relationship::id_t id) {
  if (rships_.capacity() <= id) {
    spdlog::warn("unknown relationship_id {}", id);
    throw unknown_id();
  }
  auto &r = rships_.at(id);
  return r;    
  }

  /**
   * Returns the number of occupied chunks of the underlying chunked_vec.
   */
  std::size_t num_chunks() const { return rships_.num_chunks(); }

private:
  T<relationship> rships_; // the actual list of relationships
};
#endif
