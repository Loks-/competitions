#pragma once

#include "common/base.h"
#include "common/memory/node.h"

#include <algorithm>
#include <deque>
#include <stack>
#include <type_traits>
#include <utility>

namespace memory {

/**
 * @brief Manages a pool of nodes with lazy recycling.
 *
 * This class manages a pool of nodes, providing efficient allocation and
 * deallocation through recycling. It maintains a stack of released nodes
 * that can be reused, and only creates new nodes when necessary.
 *
 * @tparam TNode The type of node to manage. Must be derived from memory::Node.
 */
template <typename TNode>
class NodesManager {
  static_assert(std::is_base_of_v<Node, TNode>,
                "TNode must be derived from memory::Node");

 public:
  using NodeType = TNode;

  // Flags indicating supported operations
  static constexpr bool support_at = true;
  static constexpr bool support_index = false;

  /**
   * @brief Constructs a nodes manager with initial capacity.
   *
   * @param initial_capacity The initial number of nodes to reserve.
   *                         Defaults to 0 if not specified.
   */
  [[nodiscard]] constexpr explicit NodesManager(size_t initial_capacity = 0)
      : nodes_(initial_capacity) {}

  /**
   * @brief Initializes the nodes manager with the specified capacity.
   *
   * This function reserves space for the specified number of nodes.
   * It's provided to maintain interface compatibility with
   * ContiguousNodesManager.
   *
   * @param initial_capacity The initial number of nodes to reserve.
   */
  constexpr void init(size_t initial_capacity) { reserve(initial_capacity); }

  /**
   * @brief Copy constructor is deleted.
   *
   * NodesManager cannot be copied because it manages raw pointers and
   * maintains internal state about node allocation.
   */
  NodesManager(const NodesManager&) = delete;

  /**
   * @brief Copy assignment operator is deleted.
   *
   * NodesManager cannot be copied because it manages raw pointers and
   * maintains internal state about node allocation.
   */
  NodesManager& operator=(const NodesManager&) = delete;

  /**
   * @brief Move constructor.
   *
   * @param other The nodes manager to move from. After the move,
   *              other will be in a valid but unspecified state.
   */
  constexpr NodesManager(NodesManager&& other) noexcept
      : nodes_(std::move(other.nodes_)),
        used_nodes_(std::exchange(other.used_nodes_, 0)),
        released_nodes_(std::move(other.released_nodes_)) {}

  /**
   * @brief Move assignment operator.
   *
   * @param other The nodes manager to move from. After the move,
   *              other will be in a valid but unspecified state.
   * @return Reference to this nodes manager.
   */
  constexpr NodesManager& operator=(NodesManager&& other) noexcept {
    if (this != &other) {
      nodes_ = std::move(other.nodes_);
      used_nodes_ = std::exchange(other.used_nodes_, 0);
      released_nodes_ = std::move(other.released_nodes_);
    }
    return *this;
  }

  /**
   * @brief Reserves space for at least the specified number of nodes.
   *
   * If the requested capacity is greater than the current capacity,
   * the capacity is increased to at least the requested size.
   *
   * @param new_capacity The minimum number of nodes to reserve.
   */
  constexpr void reserve(size_t new_capacity) {
    if (new_capacity > nodes_.size()) {
      nodes_.resize(std::max(new_capacity, 2 * nodes_.size()));
    }
  }

  /**
   * @brief Reserves additional space for the specified number of nodes.
   *
   * If the current available capacity is less than the requested number
   * of nodes, the capacity is increased to accommodate the additional nodes.
   *
   * @param additional_nodes The number of additional nodes to reserve.
   */
  constexpr void reserve_additional(size_t additional_nodes) {
    if (available_capacity() < additional_nodes) {
      reserve(used_nodes_ + additional_nodes);
    }
  }

  /**
   * @brief Creates a new node or reuses a released one.
   *
   * If there are any released nodes available, one is reused.
   * Otherwise, a new node is created from the pool.
   *
   * @return Pointer to the new or reused node.
   */
  [[nodiscard]] constexpr NodeType* create() {
    if (!released_nodes_.empty()) {
      NodeType* node = released_nodes_.top();
      node->reuse();
      released_nodes_.pop();
      return node;
    }

    reserve_additional(1);
    assert(used_nodes_ < nodes_.size());
    NodeType* node = &nodes_[used_nodes_];
    node->initialize(used_nodes_++);
    return node;
  }

  /**
   * @brief Releases a node for future reuse.
   *
   * The node is marked as released and added to the stack of available nodes.
   * The node can be reused later by calling create().
   *
   * @param node Pointer to the node to release. Must be a valid node pointer
   *             that was previously created by this manager.
   */
  constexpr void release(NodeType* node) {
    node->release();
    released_nodes_.push(node);
  }

  /**
   * @brief Returns the total capacity of the node pool.
   *
   * @return The total number of nodes that can be stored in the pool.
   */
  [[nodiscard]] constexpr size_t capacity() const noexcept {
    return nodes_.size();
  }

  /**
   * @brief Returns the number of currently used nodes.
   *
   * @return The number of nodes that are currently in use, not including
   *         nodes that have been released and are available for reuse.
   */
  [[nodiscard]] constexpr size_t used() const noexcept {
    return used_nodes_ - released_nodes_.size();
  }

  /**
   * @brief Returns the number of available nodes.
   *
   * @return The number of nodes that can be created without requiring
   *         additional memory allocation.
   */
  [[nodiscard]] constexpr size_t available_capacity() const noexcept {
    return capacity() - used();
  }

  /**
   * @brief Returns a pointer to a node by its raw index.
   *
   * @param index The index of the node in the underlying storage.
   * @return Pointer to the node at the specified index.
   */
  [[nodiscard]] constexpr NodeType* at(size_t index) { return &nodes_[index]; }

  /**
   * @brief Returns a pointer to a node by its raw index (const version).
   *
   * @param index The index of the node in the underlying storage.
   * @return Const pointer to the node at the specified index.
   */
  [[nodiscard]] constexpr const NodeType* at(size_t index) const {
    return &nodes_[index];
  }

  /**
   * @brief Gets the index of a node.
   *
   * This function is only supported in ContiguousNodesManager.
   * Using it with other node managers will result in a compile error.
   *
   * @param node Pointer to the node
   * @return Index of the node (0 in this case, but should never be called)
   */
  [[nodiscard]] constexpr size_t index(const NodeType*) {
    static_assert(sizeof(NodeType) == 0,
                  "index() is only supported in ContiguousNodesManager. "
                  "Please use ContiguousNodesManager instead of NodesManager.");
    return 0;
  }

  /**
   * @brief Removes all nodes and frees allocated memory.
   *
   * This function releases all nodes and frees the memory allocated for them.
   * After this call, the manager will be empty and ready for new allocations.
   */
  constexpr void clear_memory() {
    for (size_t i = 0; i < used_nodes_; ++i) nodes_[i].release();
    used_nodes_ = 0;
    std::stack<NodeType*>().swap(released_nodes_);
    std::deque<NodeType>().swap(nodes_);
  }

  /**
   * @brief Removes all nodes but keeps the memory allocated.
   *
   * This function releases all nodes and resets them to their initial state.
   * Unlike clear_memory(), this function does not free the memory, allowing for
   * immediate reuse of the nodes.
   */
  constexpr void clear() {
    for (size_t i = 0; i < used_nodes_; ++i) {
      nodes_[i].release();
      nodes_[i].reuse();
    }
    used_nodes_ = 0;
    std::stack<NodeType*>().swap(released_nodes_);
  }

 protected:
  std::deque<NodeType> nodes_;
  size_t used_nodes_{0};
  std::stack<NodeType*> released_nodes_;
};

}  // namespace memory
