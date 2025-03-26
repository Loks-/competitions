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
   * @param raw_index The raw index of the node.
   */
  constexpr void clear_create(unsigned) {}

  /**
   * @brief Resets the balance tree data.
   */
  constexpr void bti_reset() {}

  /**
   * @brief Copies balance tree data from another node.
   *
   * @param node The source node to copy from (unused in this implementation).
   */
  template <typename Node>
  constexpr void bti_copy(const Node*) {}

  /**
   * @brief Swaps balance tree data with another node.
   *
   * @param node The node to swap with (unused in this implementation).
   */
  template <typename Node>
  constexpr void bti_swap(Node*) {}

  /**
   * @brief Updates the subtree data.
   *
   * This function will be used only if segments are not supported.
   *
   * @param node The node to update (unused in this implementation).
   */
  template <typename Node>
  constexpr void update(const Node*) {}

  /**
   * @brief Sets the subtree data from a single node.
   *
   * @param node The node to set subtree data from.
   */
  template <typename Node>
  constexpr void set_node(const Node*) {}

  /**
   * @brief Sets the subtree data from a subtree.
   *
   * @param node The root of the subtree to set subtree data from.
   */
  template <typename Node>
  constexpr void set_subtree(const Node*) {}

  /**
   * @brief Adds the subtree data from a single node.
   *
   * This function will be used only if aggregation is order independent.
   *
   * @param node The node to add subtree data from.
   */
  template <typename Node>
  constexpr void add_node(const Node*) {}

  /**
   * @brief Adds the subtree data from a subtree.
   *
   * This function will be used only if aggregation is order independent.
   *
   * @param node The root of the subtree to add subtree data from.
   */
  template <typename Node>
  constexpr void add_subtree(const Node*) {}

  /**
   * @brief Adds the subtree data from a single node.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The node to add subtree data from.
   */
  template <typename Node>
  constexpr void add_left_node(const Node*) {}

  /**
   * @brief Adds the subtree data from a single node.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The node to add subtree data from.
   */
  template <typename Node>
  constexpr void add_right_node(const Node*) {}

  /**
   * @brief Adds the subtree data from a subtree.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The root of the subtree to add subtree data from.
   */
  template <typename Node>
  constexpr void add_left_subtree(const Node*) {}

  /**
   * @brief Adds the subtree data from a subtree.
   *
   * This function will be used only if aggregation is order dependent.
   *
   * @param node The root of the subtree to add subtree data from.
   */
  template <typename Node>
  constexpr void add_right_subtree(const Node*) {}

  /**
   * @brief Handles node insertion in the subtree data.
   *
   * This function will be used only if insert operations are supported
   * (support_insert_node is true) for all aggregation operations.
   *
   * @param node The node being inserted.
   */
  template <typename Node>
  constexpr void insert_node(const Node*) {}

  /**
   * @brief Handles subtree insertion in the subtree data.
   *
   * This function will be used only if insert operations are supported
   * (support_insert_subtree is true) for all aggregation operations.
   *
   * @param node The root of the subtree being inserted.
   */
  template <typename Node>
  constexpr void insert_subtree(const Node*) {}

  /**
   * @brief Handles node removal from the subtree data.
   *
   * This function will be used only if remove operations are supported
   * (support_remove_node is true) for all aggregation operations.
   *
   * @param node The node being removed.
   */
  template <typename Node>
  constexpr void remove_node(const Node*) {}
};

}  // namespace subtree_data
}  // namespace bst
