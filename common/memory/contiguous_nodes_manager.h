#pragma once

#include "common/base.h"
#include "common/memory/node.h"

#include <stack>
#include <type_traits>
#include <utility>
#include <vector>

namespace memory {

/**
 * @brief Manages a fixed-size pool of nodes with contiguous memory layout.
 *
 * This class manages a pool of nodes that are allocated in a contiguous
 * memory block. Unlike NodesManager, this class does not support dynamic
 * resizing and requires the maximum number of nodes to be specified at
 * construction time.
 *
 * The contiguous memory layout allows for efficient pointer arithmetic
 * when accessing nodes by index, but comes at the cost of fixed size
 * and potential memory waste if the maximum size is overestimated.
 *
 * @tparam TNode The type of node to manage. Must be derived from memory::Node.
 */
template <class TNode>
class ContiguousNodesManager {
  static_assert(std::is_base_of_v<Node, TNode>,
                "TNode must be derived from memory::Node");

 public:
  using NodeType = TNode;

  /**
   * @brief Constructs a nodes manager with fixed capacity.
   *
   * @param max_nodes The maximum number of nodes that can be managed.
   *                 This determines the size of the contiguous memory block.
   */
  [[nodiscard]] constexpr explicit ContiguousNodesManager(
      size_t max_nodes = 0) {
    reset(max_nodes);
  }

  /**
   * @brief Move constructor.
   *
   * @param other The nodes manager to move from. After the move,
   *              other will be in a valid but unspecified state.
   */
  constexpr ContiguousNodesManager(ContiguousNodesManager&& other) noexcept
      : nodes_(std::move(other.nodes_)),
        used_nodes_(std::exchange(other.used_nodes_, 0)),
        released_nodes_(std::move(other.released_nodes_)),
        first_(std::exchange(other.first_, nullptr)) {}

  /**
   * @brief Move assignment operator.
   *
   * @param other The nodes manager to move from. After the move,
   *              other will be in a valid but unspecified state.
   * @return Reference to this nodes manager.
   */
  constexpr ContiguousNodesManager& operator=(
      ContiguousNodesManager&& other) noexcept {
    if (this != &other) {
      nodes_ = std::move(other.nodes_);
      used_nodes_ = std::exchange(other.used_nodes_, 0);
      released_nodes_ = std::move(other.released_nodes_);
      first_ = std::exchange(other.first_, nullptr);
    }
    return *this;
  }

  /**
   * @brief Reserves space for the specified number of nodes.
   *
   * This function is a no-op in this manager as the size is fixed.
   * It only verifies that the requested size does not exceed the
   * maximum capacity.
   *
   * @param new_max_nodes The requested number of nodes. Must not exceed
   *                     the maximum capacity specified at construction.
   */
  constexpr void reserve([[maybe_unused]] size_t new_max_nodes) {
    assert(new_max_nodes <= nodes_.size());
  }

  /**
   * @brief Reserves additional space for the specified number of nodes.
   *
   * This function is a no-op in this manager as the size is fixed.
   * It only verifies that the requested additional nodes do not exceed
   * the available capacity.
   *
   * @param new_nodes The number of additional nodes to reserve. Must not
   *                 exceed the available capacity.
   */
  constexpr void reserve_additional([[maybe_unused]] size_t new_nodes) {
    assert(new_nodes <= available_capacity());
  }

  /**
   * @brief Creates a new node or reuses a released one.
   *
   * If there are any released nodes available, one is reused.
   * Otherwise, a new node is created from the contiguous block.
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
   * @return The maximum number of nodes that can be managed.
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
   * @return The number of nodes that can be created without exceeding
   *         the maximum capacity.
   */
  [[nodiscard]] constexpr size_t available_capacity() const noexcept {
    return capacity() - used();
  }

  /**
   * @brief Returns a pointer to a node by its raw index.
   *
   * This function uses pointer arithmetic for efficient access to nodes.
   *
   * @param index The index of the node in the contiguous block.
   * @return Pointer to the node at the specified index.
   */
  [[nodiscard]] constexpr NodeType* at(size_t index) { return first_ + index; }

  /**
   * @brief Returns a pointer to a node by its raw index (const version).
   *
   * This function uses pointer arithmetic for efficient access to nodes.
   *
   * @param index The index of the node in the contiguous block.
   * @return Const pointer to the node at the specified index.
   */
  [[nodiscard]] constexpr const NodeType* at(size_t index) const {
    return first_ + index;
  }

  /**
   * @brief Returns the index of a node in the contiguous block.
   *
   * This function uses pointer arithmetic to compute the index of a node.
   *
   * @param node Pointer to the node to get the index for.
   * @return The index of the node in the contiguous block.
   */
  [[nodiscard]] constexpr size_t index(const NodeType* node) const {
    return node - first_;
  }

  /**
   * @brief Removes all nodes and frees allocated memory.
   *
   * This function releases all nodes and frees the memory allocated for them.
   * After this call, the manager will be empty and ready for new allocations.
   */
  constexpr void clear_memory() {
    std::stack<NodeType*>().swap(released_nodes_);
    used_nodes_ = 0;
    for (NodeType& node : nodes_) node.release();
    nodes_.clear();
    first_ = nullptr;
  }

  /**
   * @brief Removes all nodes but keeps the memory allocated.
   *
   * This function releases all nodes and resets them to their initial state.
   * Unlike clear_memory(), this function does not free the memory, allowing for
   * immediate reuse of the nodes.
   */
  constexpr void clear() {
    std::stack<NodeType*>().swap(released_nodes_);
    used_nodes_ = 0;
    for (NodeType& node : nodes_) {
      node.release();
      node.reuse();
    }
  }

  /**
   * @brief Resets the manager with a new maximum node count.
   *
   * This function reallocates the memory block and resets all internal
   * state. All existing nodes are invalidated.
   *
   * @param max_nodes The new maximum number of nodes to manage.
   */
  constexpr void reset(size_t max_nodes) {
    clear_memory();
    nodes_.resize(max_nodes);
    first_ = nodes_.empty() ? nullptr : &nodes_[0];
  }

 protected:
  std::vector<NodeType> nodes_;
  size_t used_nodes_;
  std::stack<NodeType*> released_nodes_;
  NodeType* first_;
};

}  // namespace memory
