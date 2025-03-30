#pragma once

namespace bst {
namespace base {

/**
 * @brief Gets the depth of a node relative to the root.
 *
 * This function calculates how deep a node is in the tree, with the root
 * node having depth 1. The depth is calculated by counting the number of
 * parent links from the node to the root.
 *
 * Since this is a read-only operation that only traverses parent links,
 * no deferred computations are applied. If a pointer to a node is available,
 * it is assumed that all deferred computations have already been applied.
 *
 * @tparam TNode The BST node type.
 * @param node The node to calculate depth for.
 * @return The depth of the node (1 for root, 2 for root's children, etc.),
 *         or 0 if the node is nullptr.
 */
template <typename TNode>
[[nodiscard]] constexpr unsigned deep(const TNode* node) {
  // Count parent links from node to root
  static_assert(TNode::has_parent, "Node must have parent pointer enabled");
  unsigned depth = 0;
  for (; node; node = node->parent) ++depth;
  return depth;
}

}  // namespace base
}  // namespace bst
