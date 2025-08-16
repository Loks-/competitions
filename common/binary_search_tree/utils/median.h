#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/at.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace utils {

/**
 * @brief Calculates the median value of a binary search tree.
 *
 * This function calculates the median value of a binary search tree by finding
 * the middle element(s) in the inorder traversal. For odd-sized trees, it
 * returns the middle element. For even-sized trees, it returns the average
 * of the two middle elements.
 *
 * The function requires that the tree's subtree data contains size information
 * to efficiently locate the median position.
 *
 * @tparam TNode The BST node type.
 * @tparam TValue The return value type (should be convertible from the node's
 * key type).
 * @param root The root of the tree to calculate the median for.
 * @return The median value of the tree.
 * @pre root must not be nullptr.
 * @pre The tree's subtree data must contain size information.
 */
template <typename TNode, typename TValue>
[[nodiscard]] constexpr TValue median(TNode* root) {
  static_assert(TNode::SubtreeDataType::has_size,
                "subtree data should contain size");
  assert(root && "root must not be nullptr");

  const size_t size = bst::subtree_data::size(root);

  if (size & 1) {
    // Odd size: return the middle element
    return TValue(bst::base::at(root, size / 2)->key);
  } else {
    // Even size: return the average of the two middle elements
    const TValue left_middle = TValue(bst::base::at(root, size / 2 - 1)->key);
    const TValue right_middle = TValue(bst::base::at(root, size / 2)->key);
    return (left_middle + right_middle) / TValue(2);
  }
}

}  // namespace utils
}  // namespace bst
