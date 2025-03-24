#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/base.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains the size of subtrees in a BST.
 *
 * This component tracks the number of nodes in each subtree and supports
 * all basic operations including insertion, removal, and segment operations.
 * The size is maintained efficiently through local updates during tree
 * modifications.
 */
class Size : public Base {
 public:
  using Self = Size;

  /**
   * @brief Component capability flags.
   *
   * Size component supports all operations as it only needs to
   * track node counts, which can be updated locally.
   */
  static constexpr bool has_size = true;
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;
  static constexpr bool support_remove_node = true;

  /**
   * @brief Gets the size from a SubtreeData instance.
   *
   * Helper function to access the size directly from a SubtreeData object.
   * Assumes that Size is one of the aggregators in the tuple.
   *
   * @tparam TAggregators The tuple of aggregator types.
   * @param subtree_data The SubtreeData instance to get size from.
   * @return The size stored in the subtree data.
   */
  template <typename TAggregators>
  static constexpr unsigned get(
      const bst::base::SubtreeData<TAggregators>& subtree_data) {
    return subtree_data.template get<Self>().size;
  }

  /**
   * @brief Gets the size of a subtree.
   *
   * Helper function to access the size of a subtree through its root node.
   * Returns 0 for null nodes.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to get size from.
   * @return The size of the subtree.
   */
  template <typename Node>
  static constexpr unsigned get(const Node* node) {
    return node ? get(node->subtree_data) : 0;
  }

  /**
   * @brief Sets size to 1 for a single node.
   *
   * For a single node, the size is equal to 1.
   *
   * @tparam Node The BST node type.
   * @param node The node to set size from.
   */
  template <typename Node>
  constexpr void set_node([[maybe_unused]] const Node* node) {
    assert(node);
    size = 1;
  }

  /**
   * @brief Copies size from another subtree.
   *
   * Used when replacing one subtree with another to maintain correct size.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to copy size from.
   */
  template <typename Node>
  constexpr void set_subtree(const Node* node) {
    assert(node);
    size = get(node);
  }

  /**
   * @brief Adds node's size (1) to the total.
   *
   * Used in order-independent operations where a single node
   * is being added to the subtree.
   *
   * @tparam Node The BST node type.
   * @param node The node being added.
   */
  template <typename Node>
  constexpr void add_node([[maybe_unused]] const Node* node) {
    assert(node);
    size += 1;
  }

  /**
   * @brief Adds size of another subtree.
   *
   * Used in order-independent operations where an entire subtree
   * is being added to the current subtree.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree being added.
   */
  template <typename Node>
  constexpr void add_subtree(const Node* node) {
    assert(node);
    size += get(node);
  }

  /**
   * @brief Adds node's size (1) during insertion.
   *
   * Similar to add_node but specifically used during tree
   * modification operations.
   *
   * @tparam Node The BST node type.
   * @param node The node being inserted.
   */
  template <typename Node>
  constexpr void insert_node([[maybe_unused]] const Node* node) {
    assert(node);
    size += 1;
  }

  /**
   * @brief Adds size of inserted subtree.
   *
   * Similar to add_subtree but specifically used during tree
   * modification operations.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree being inserted.
   */
  template <typename Node>
  constexpr void insert_subtree(const Node* node) {
    assert(node);
    size += get(node);
  }

  /**
   * @brief Subtracts node's size (1) during removal.
   *
   * Updates the subtree size when a node is being removed
   * from the tree.
   *
   * @tparam Node The BST node type.
   * @param node The node being removed.
   */
  template <typename Node>
  constexpr void remove_node([[maybe_unused]] const Node* node) {
    assert(node);
    size -= 1;
  }

 protected:
  /**
   * @brief The size of the subtree (number of nodes).
   *
   * This value represents the total number of nodes in the subtree,
   * including the root node.
   */
  unsigned size{0};
};

/**
 * @brief Gets the size from a SubtreeData instance.
 *
 * Free function to access the size directly from a SubtreeData object.
 * Assumes that Size is one of the aggregators in the tuple.
 *
 * @tparam TAggregators The tuple of aggregator types.
 * @param subtree_data The SubtreeData instance to get size from.
 * @return The size stored in the subtree data.
 */
template <typename TAggregators>
constexpr unsigned size(
    const bst::base::SubtreeData<TAggregators>& subtree_data) {
  return Size::get(subtree_data);
}

/**
 * @brief Gets the size of a subtree.
 *
 * Free function to access the size of a subtree through its root node.
 * Returns 0 for null nodes.
 *
 * @tparam Node The BST node type.
 * @param node The root of the subtree to get size from.
 * @return The size of the subtree.
 */
template <typename Node>
constexpr unsigned size(const Node* node) {
  return Size::get(node);
}

}  // namespace subtree_data
}  // namespace bst
