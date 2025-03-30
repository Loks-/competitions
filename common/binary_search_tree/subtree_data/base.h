#pragma once

namespace bst {
namespace subtree_data {

/**
 * @brief A base class for BST subtree data that defines the interface.
 *
 * This class serves as a base for all subtree data components, providing
 * a complete interface with default no-op implementations and compile-time
 * flags to indicate supported operations.
 *
 * Components can override the flags and methods they support, leaving
 * others at their default values.
 */
class Base {
 public:
  /**
   * @brief Flag indicating whether this subtree data class uses node data.
   *
   * When true, the component requires access to node->data for its operations.
   * This can be used to optimize operations when no components need data
   * access.
   */
  static constexpr bool use_data = false;

  /**
   * @brief Flag indicating whether this subtree data class uses node keys.
   *
   * When true, the component requires access to node->key for its operations.
   * This can be used to optimize operations when no components need key access.
   */
  static constexpr bool use_keys = false;

  /**
   * @brief Flag indicating whether this subtree data class depends on tree
   * structure.
   *
   * When true, the component requires access to tree structure information
   * (like node heights, depths, or other structural properties) for its
   * operations. This can be used to optimize operations when no components need
   * structural information.
   */
  static constexpr bool use_tree_structure = false;

  /**
   * @brief Flag indicating whether this subtree data class's value depends on
   * the order of nodes in the subtree.
   *
   * When true, the component's aggregated value will change if the order of
   * nodes in the subtree changes. For example, a sum aggregator is order
   * independent (order doesn't matter), while a sequence-specific aggregator
   * would be order dependent. This flag helps optimize updates by determining
   * when subtree data needs to be recalculated after structural changes.
   */
  static constexpr bool use_nodes_order = false;

  /**
   * @brief Flag indicating whether this subtree data class maintains subtree
   * size.
   *
   * Set to false as by default no size information is maintained.
   */
  static constexpr bool has_size = false;

  /**
   * @brief Flag indicating whether this subtree data class supports segment
   * operations.
   *
   * Set to false as by default segment operations are not supported.
   */
  static constexpr bool support_segment = false;

  /**
   * @brief Flag indicating whether this subtree data class is
   * order-independent.
   *
   * Set to true as by default the subtree data is order-independent.
   */
  static constexpr bool order_independent = true;

  /**
   * @brief Flag indicating whether this subtree data class supports insert node
   * operations.
   *
   * Set to false as by default insert operations are not supported.
   */
  static constexpr bool support_insert_node = false;

  /**
   * @brief Flag indicating whether this subtree data class supports insert
   * subtree operations.
   *
   * Set to false as by default insert operations are not supported.
   */
  static constexpr bool support_insert_subtree = false;

  /**
   * @brief Flag indicating whether this subtree data class supports remove node
   * operations.
   *
   * Set to false as by default remove operations are not supported.
   */
  static constexpr bool support_remove_node = false;

 public:
  /**
   * @brief Clears and initializes the subtree data for a new node.
   *
   * @param index The unique index of the node.
   */
  constexpr void initialize(unsigned) {}

  /**
   * @brief Resets the balance tree data.
   */
  constexpr void bti_reset() {}

  /**
   * @brief Copies balance tree data from another node.
   *
   * @param node The source node to copy from (unused in this implementation).
   */
  template <typename TNode>
  constexpr void bti_copy(const TNode*) {}

  /**
   * @brief Swaps balance tree data with another node.
   *
   * @param node The node to swap with (unused in this implementation).
   */
  template <typename TNode>
  constexpr void bti_swap(TNode*) {}

  /**
   * @brief Updates the subtree data.
   *
   * This function will be used only if segments are not supported.
   *
   * @param node The node to update (unused in this implementation).
   */
  template <typename TNode>
  constexpr void update(const TNode*) {}

  /**
   * @brief Sets the subtree data from a single node.
   *
   * @param node The node to set subtree data from.
   */
  template <typename TNode>
  constexpr void set_node(const TNode*) {}

  /**
   * @brief Sets the subtree data from a subtree.
   *
   * @param node The root of the subtree to set subtree data from.
   */
  template <typename TNode>
  constexpr void set_subtree(const TNode*) {}

  /**
   * @brief Adds the subtree data from a single node.
   *
   * This function will be used only if aggregation is order independent.
   *
   * @param node The node to add subtree data from.
   */
  template <typename TNode>
  constexpr void add_node(const TNode*) {}

  /**
   * @brief Adds the subtree data from a subtree.
   *
   * This function will be used only if aggregation is order independent.
   *
   * @param node The root of the subtree to add subtree data from.
   */
  template <typename TNode>
  constexpr void add_subtree(const TNode*) {}

  /**
   * @brief Adds the subtree data from a single node.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The node to add subtree data from.
   */
  template <typename TNode>
  constexpr void add_left_node(const TNode*) {}

  /**
   * @brief Adds the subtree data from a single node.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The node to add subtree data from.
   */
  template <typename TNode>
  constexpr void add_right_node(const TNode*) {}

  /**
   * @brief Adds the subtree data from a subtree.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The root of the subtree to add subtree data from.
   */
  template <typename TNode>
  constexpr void add_left_subtree(const TNode*) {}

  /**
   * @brief Adds the subtree data from a subtree.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The root of the subtree to add subtree data from.
   */
  template <typename TNode>
  constexpr void add_right_subtree(const TNode*) {}

  /**
   * @brief Handles node insertion in the subtree data.
   *
   * This function will be used only if insert operations are supported
   * (support_insert_node is true) for all aggregation operations.
   *
   * @param node The node being inserted.
   */
  template <typename TNode>
  constexpr void insert_node(const TNode*) {}

  /**
   * @brief Handles subtree insertion in the subtree data.
   *
   * This function will be used only if insert operations are supported
   * (support_insert_subtree is true) for all aggregation operations.
   *
   * @param node The root of the subtree being inserted.
   */
  template <typename TNode>
  constexpr void insert_subtree(const TNode*) {}

  /**
   * @brief Handles node removal from the subtree data.
   *
   * This function will be used only if remove operations are supported
   * (support_remove_node is true) for all aggregation operations.
   *
   * @param node The node being removed.
   */
  template <typename TNode>
  constexpr void remove_node(const TNode*) {}
};

}  // namespace subtree_data
}  // namespace bst
