#pragma once

namespace bst {
namespace subtree_data {

/**
 * @brief A base class for BST subtree data that stores nothing.
 *
 * This class serves as a base case for BST subtree data classes.
 * It provides empty implementations of all required methods and sets
 * appropriate flags to indicate it stores no information.
 */
class None {
 public:
  using Self = None;

  /**
   * @brief Flag indicating this is a None subtree data class.
   *
   * Used to identify that this class stores no information.
   */
  static constexpr bool is_none = true;

  /**
   * @brief Flag indicating whether this subtree data class uses node data.
   *
   * Set to false as this class stores no information.
   */
  static constexpr bool use_data = false;

  /**
   * @brief Flag indicating whether this subtree data class uses node keys.
   *
   * Set to false as this class stores no information.
   */
  static constexpr bool use_keys = false;

  /**
   * @brief Flag indicating whether this subtree data class maintains subtree
   * size.
   *
   * Set to false as this class stores no information.
   */
  static constexpr bool has_size = false;

  /**
   * @brief Flag indicating whether this subtree data class supports insert
   * operations.
   *
   * Used for external tree implementations.
   */
  static constexpr bool support_insert = false;

  /**
   * @brief Flag indicating whether this subtree data class supports remove
   * operations.
   *
   * Used for external tree implementations.
   */
  static constexpr bool support_remove = false;

 public:
  /**
   * @brief Clears and initializes the subtree data for a new node.
   *
   * @param size The size of the subtree (unused in this implementation).
   */
  constexpr void clear_create(unsigned) {}

  /**
   * @brief Updates the subtree data.
   *
   * This is an empty implementation as this class stores no information.
   * External functions should not assume this is constexpr.
   *
   * @param node The node to update (unused in this implementation).
   */
  template <typename Node>
  constexpr void update(const Node*) {}

  /**
   * @brief Resets the balance tree data.
   *
   * This is an empty implementation as this class stores no information.
   */
  constexpr void bti_reset() {}

  /**
   * @brief Copies balance tree data from another node.
   *
   * This is an empty implementation as this class stores no information.
   *
   * @param node The source node to copy from (unused in this implementation).
   */
  template <typename Node>
  constexpr void bti_copy(const Node*) {}

  /**
   * @brief Swaps balance tree data with another node.
   *
   * This is an empty implementation as this class stores no information.
   *
   * @param node The node to swap with (unused in this implementation).
   */
  template <typename Node>
  constexpr void bti_swap(Node*) {}
};
}  // namespace subtree_data
}  // namespace bst
