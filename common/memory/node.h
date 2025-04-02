#pragma once

namespace memory {

/**
 * @brief Base class for memory-managed nodes in various data structures.
 *
 * This class provides the interface for memory management of nodes in data
 * structures like binary search trees, treaps, etc. It works in conjunction
 * with a NodesManager to implement a memory pool pattern, avoiding frequent
 * allocations and deallocations.
 *
 * The class provides three key lifecycle methods that are called by the
 * NodesManager at different stages of a node's lifetime:
 * - initialize: Called when memory is first allocated for a node
 * - release: Called when a node is returned to the free pool
 * - reuse: Called when a node is retrieved from the free pool
 *
 * Derived classes can override these methods to implement their own
 * initialization and cleanup logic.
 */
class Node {
 public:
  /**
   * @brief Initializes a newly allocated node.
   *
   * This method is called by NodesManager when memory is first allocated
   * for a node. It provides a unique index for the node, which can be used
   * for various purposes (e.g., random number generation in treaps).
   *
   * The default implementation does nothing, but derived classes can override
   * this to initialize their specific fields.
   *
   * @param index A unique index assigned to this node by NodesManager
   */
  constexpr void initialize(unsigned) noexcept {}

  /**
   * @brief Prepares a node for return to the free pool.
   *
   * This method is called by NodesManager when a node is being released
   * back to the free pool. It's not a real memory deallocation, but rather
   * a way to mark the node as available for reuse.
   *
   * The default implementation does nothing, but derived classes can override
   * this to clean up their specific fields.
   */
  constexpr void release() noexcept {}

  /**
   * @brief Prepares a node for reuse from the free pool.
   *
   * This method is called by NodesManager when a node is being retrieved
   * from the free pool for reuse. It's similar to initialize but might
   * skip some initializations for efficiency (e.g., in treaps where not
   * all fields need to be reset).
   *
   * The default implementation does nothing, but derived classes can override
   * this to implement their specific reuse logic.
   */
  constexpr void reuse() noexcept {}
};

}  // namespace memory
