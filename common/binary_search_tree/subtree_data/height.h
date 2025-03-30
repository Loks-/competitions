#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/base.h"

#include <algorithm>

namespace bst {
namespace subtree_data {

/**
 * @brief A component that maintains the height of subtrees in a BST.
 *
 * This component tracks the height of each subtree, where height is defined
 * as the length of the longest path from the root to any leaf. A single node
 * has height 1, and an empty subtree (nullptr) has height 0.
 */
class Height : public Base {
 public:
  using Self = Height;

  /**
   * @brief Component capability flags.
   *
   * Height component depends on tree structure since it needs to know
   * the heights of child subtrees to compute a subtree's height. Insert and
   * remove operations are not supported as they would require recomputation of
   * heights along the path.
   */
  static constexpr bool use_tree_structure = true;

  /**
   * @brief Gets the height of a subtree.
   *
   * Helper function to access the height of a subtree through its root node.
   * Returns 0 for null nodes.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to get height from.
   * @return The height of the subtree.
   */
  template <typename TNode>
  static constexpr unsigned get(const TNode* node) {
    return node ? node->subtree_data.template get<Self>().height : 0;
  }

  /**
   * @brief Updates the height of a subtree.
   *
   * The height of a subtree is one more than the maximum height of its
   * child subtrees. If a child is null, its height is considered 0.
   *
   * @tparam TNode The BST node type.
   * @param node The root of the subtree to update height for.
   */
  template <typename TNode>
  constexpr void update(const TNode* node) {
    assert(node);
    height = 1 + std::max(get(node->left), get(node->right));
  }

 protected:
  /**
   * @brief The height of the subtree.
   *
   * This value represents the length of the longest path from the root
   * to any leaf in the subtree. A leaf node has height 1, and an internal
   * node's height is one more than the maximum height of its child subtrees.
   * A null node has height 0.
   */
  unsigned height{0};
};

/**
 * @brief Gets the height of a subtree.
 *
 * Free function to access the height of a subtree through its root node.
 * Returns 0 for null nodes.
 *
 * @tparam TNode The BST node type.
 * @param node The root of the subtree to get height from.
 * @return The height of the subtree.
 */
template <typename TNode>
constexpr unsigned height(const TNode* node) {
  return Height::get(node);
}

}  // namespace subtree_data
}  // namespace bst
