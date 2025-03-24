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
 * has height 1, and an empty subtree has height 0.
 */
class Height : public Base {
 public:
  using Self = Height;

  /**
   * @brief Component capability flags.
   *
   * Height component doesn't require access to node data or keys,
   * and supports only basic update operations. Insert and remove
   * operations are not supported as they would require recomputation
   * of heights along the path.
   */

  /**
   * @brief Gets the height of a subtree.
   *
   * Helper function to access the height of a subtree through its root node.
   * Returns 0 for null nodes.
   *
   * @tparam Node The BST node type.
   * @param node The root of the subtree to get height from.
   * @return The height of the subtree.
   */
  template <typename Node>
  static constexpr unsigned get(const Node* node) {
    return node ? node->subtree_data.template get<Self>().height : 0;
  }

  /**
   * @brief Updates the height of a subtree.
   *
   * Computes the height as 1 plus the maximum height of the left and right
   * subtrees. For null children, their height is considered 0.
   *
   * @tparam Node The BST node type.
   * @param node The node to update height for.
   */
  template <typename Node>
  constexpr void update(const Node* node) {
    assert(node);
    height = 1 + std::max(get(node->l), get(node->r));
  }

 protected:
  /**
   * @brief The height of the subtree.
   *
   * For a leaf node, height = 1.
   * For an internal node, height = 1 + max(left_height, right_height).
   * For an empty subtree (null node), height = 0.
   */
  unsigned height{0};
};

/**
 * @brief Gets the height of a subtree.
 *
 * Free function to access the height of a subtree through its root node.
 * Returns 0 for null nodes.
 *
 * @tparam Node The BST node type.
 * @param node The root of the subtree to get height from.
 * @return The height of the subtree.
 */
template <typename Node>
constexpr unsigned height(const Node* node) {
  return Height::get(node);
}

}  // namespace subtree_data
}  // namespace bst
