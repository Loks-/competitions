#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/base.h"

namespace bst {
namespace subtree_data {

/**
 * @brief A component that tracks whether a subtree is empty in a BST.
 *
 * This component maintains a boolean flag indicating whether a subtree
 * contains any nodes. It supports all basic operations including insertion,
 * removal, and segment operations. The empty state is maintained efficiently
 * through local updates during tree modifications.
 *
 * This is particularly useful for interval operations where you need to
 * quickly check if a segment of the tree contains any nodes.
 */
class IsEmpty : public Base {
 public:
  using Self = IsEmpty;

  /**
   * @brief Component capability flags.
   */
  static constexpr bool support_segment = true;
  static constexpr bool support_insert_node = true;
  static constexpr bool support_insert_subtree = true;
  static constexpr bool support_remove_node = true;

  /**
   * @brief Gets the empty state from a SubtreeData instance.
   *
   * Helper function to access the empty state directly from a SubtreeData
   * object. Assumes that IsEmpty is one of the aggregators in the tuple.
   *
   * @tparam TAggregators The tuple of aggregator types.
   * @param subtree_data The SubtreeData instance to get empty state from.
   * @return true if the subtree is empty, false otherwise.
   */
  template <typename TAggregators>
  static constexpr bool get(
      const bst::base::SubtreeData<TAggregators>& subtree_data) {
    return subtree_data.template get<Self>().is_empty;
  }

  /**
   * @brief Sets empty state for a single node.
   *
   * For a single node, the subtree is not empty.
   *
   * @tparam TNode The BST node type.
   * @param node The node to set empty state from.
   */
  template <typename TNode>
  constexpr void set_node([[maybe_unused]] const TNode* node) {
    assert(node);
    assert(is_empty);
    is_empty = false;
  }

  /**
   * @brief Set empty state for a subtree.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to copy empty state from.
   */
  template <typename TNode>
  constexpr void set_subtree([[maybe_unused]] const TNode* node) {
    assert(node);
    assert(is_empty);
    is_empty = false;
  }

  /**
   * @brief Updates empty state when adding a node.
   *
   * @tparam TNode The BST node type.
   * @param node The node being added.
   */
  template <typename TNode>
  constexpr void add_node([[maybe_unused]] const TNode* node) {
    assert(node);
    assert(!is_empty);
  }

  /**
   * @brief Updates empty state when adding a subtree.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree being added.
   */
  template <typename TNode>
  constexpr void add_subtree([[maybe_unused]] const TNode* node) {
    assert(node);
    assert(!is_empty);
  }

  /**
   * @brief Updates empty state during node insertion.
   *
   * @tparam TNode The BST node type.
   * @param node The node being inserted.
   */
  template <typename TNode>
  constexpr void insert_node([[maybe_unused]] const TNode* node) {
    assert(node);
    assert(!is_empty);
  }

  /**
   * @brief Updates empty state during subtree insertion.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree being inserted.
   */
  template <typename TNode>
  constexpr void insert_subtree([[maybe_unused]] const TNode* node) {
    assert(node);
    assert(!is_empty);
  }

  /**
   * @brief Updates empty state during node removal.
   *
   * @tparam TNode The BST node type.
   * @param node The node being removed.
   */
  template <typename TNode>
  constexpr void remove_node([[maybe_unused]] const TNode* node) {
    assert(node);
    assert(!is_empty);
  }

 protected:
  /**
   * @brief The empty state of the subtree.
   *
   * This value represents whether the subtree contains any nodes.
   * true indicates an empty subtree, false indicates a non-empty subtree.
   */
  bool is_empty{true};
};

}  // namespace subtree_data
}  // namespace bst
