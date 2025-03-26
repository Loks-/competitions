#pragma once

namespace bst {
namespace deferred {

/**
 * @brief Base class for deferred computation management in BST operations.
 *
 * This class provides a base interface for managing deferred computations in
 * binary search trees. It allows operations to be queued and applied later,
 * which can improve performance by batching multiple operations together.
 *
 * The class tracks the nature of deferred computations through flags:
 * - modify_data: indicates if node data will be modified
 * - modify_keys: indicates if node keys will be modified
 * - modify_tree: indicates if tree structure will be modified
 *
 * These flags are used to optimize tree operations by avoiding unnecessary
 * updates or traversals when possible.
 */
class Base {
 public:
  /**
   * @brief Flag indicating if deferred operations modify node data.
   *
   * When true, indicates that pending operations will modify the data
   * stored in tree nodes. Used for optimization purposes.
   */
  static constexpr bool modify_data = false;

  /**
   * @brief Flag indicating if deferred operations modify node keys.
   *
   * When true, indicates that pending operations will modify the keys
   * of tree nodes. Used for optimization purposes.
   */
  static constexpr bool modify_keys = false;

  /**
   * @brief Flag indicating if deferred operations modify tree structure.
   *
   * When true, indicates that pending operations will modify the
   * structure of the tree (e.g., rotations, rebalancing). Used for
   * optimization purposes.
   */
  static constexpr bool modify_tree = false;

  /**
   * @brief Resets the deferred computation state to empty.
   *
   * This function clears any pending deferred computations and resets
   * the internal state. After calling this function, apply_required() should
   * return false.
   */
  constexpr void clear() {}

  /**
   * @brief Checks if there are any deferred computations that need to be
   * applied.
   *
   * This function indicates whether there are any pending deferred computations
   * that need to be processed by calling apply(). Returns true if there are
   * computations waiting to be applied, false if all computations have been
   * processed.
   *
   * @return true if there are pending computations to apply, false otherwise.
   */
  constexpr bool apply_required() const { return false; }

  /**
   * @brief Applies all pending deferred computations.
   *
   * This function processes all queued computations. After successful
   * application, apply_required() should return false.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to apply computations to.
   */
  template <class Node>
  constexpr void apply(Node*) {}

  /**
   * @brief Static interface for adding deferred computations.
   *
   * This is a common interface that all deferred computation classes must
   * implement. It provides a standardized way to queue deferred operations
   * without needing to:
   * 1. Get an instance of the specific deferred class from node->deferred
   * 2. Know the specific method name for each deferred class
   *
   * Each derived class should implement this method to call its specific
   * operation (e.g., reverse_subtree, add_value, etc.).
   *
   * @tparam Node The BST node type.
   * @tparam ValueType The type of value to add (bool for Reverse, numeric for
   * AddEach, etc.).
   * @param node The root of the subtree to apply computations to.
   * @param value The value to be used in the deferred computation.
   */
  template <class Node, typename ValueType>
  static constexpr void add(Node*, const ValueType&) {}

  /**
   * @brief Notifies that a subtree has been reversed.
   *
   * This function is called when a subtree is reversed, allowing deferred
   * computations to update their state accordingly. By default, it does
   * nothing. Derived classes that need to track subtree reversals should
   * override this.
   *
   * @tparam Node The BST node type.
   * @param node The root of the reversed subtree.
   */
  template <class Node>
  constexpr void reverse_subtree(Node*) {}
};

}  // namespace deferred
}  // namespace bst
