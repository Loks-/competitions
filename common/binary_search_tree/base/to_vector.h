#pragma once

#include "common/binary_search_tree/subtree_data/size.h"

#include <utility>
#include <vector>

namespace bst {
namespace base {

/**
 * @brief Collects node values from a subtree in inorder traversal order.
 *
 * This function traverses the subtree in inorder order (left subtree, then
 * node, then right subtree) and collects values by applying the provided
 * function to each node. It can optionally apply deferred computations to
 * ensure node state is up to date.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction. When called with a const node and
 * apply_deferred=false, no deferred computations will be applied.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @tparam F The type of the function to apply to each node.
 * @param node The root of the subtree to traverse.
 * @param output Vector to collect the results.
 * @param f Function to apply to each node.
 */
template <bool apply_deferred = true, typename TNode, typename F>
constexpr void collect_inorder(
    TNode* node,
    std::vector<typename std::invoke_result<F, TNode*>::type>& output, F&& f) {
  if (!node) return;

  // Apply deferred computations if requested
  if constexpr (apply_deferred) node->apply_deferred();

  collect_inorder<apply_deferred>(node->left, output, std::forward<F>(f));
  output.push_back(f(node));
  collect_inorder<apply_deferred>(node->right, output, std::forward<F>(f));
}

/**
 * @brief Converts a BST to a vector using inorder traversal.
 *
 * This function traverses the tree in inorder order (left subtree, then node,
 * then right subtree) and collects values by applying the provided function to
 * each node. It can optionally apply deferred computations to ensure node state
 * is up to date.
 *
 * The function automatically handles both const and non-const nodes through
 * template parameter deduction. When called with a const node and
 * apply_deferred=false, no deferred computations will be applied.
 *
 * @tparam apply_deferred Whether to apply deferred computations to the node.
 *                        Defaults to true to ensure node state is up to date
 *                        when accessed externally.
 * @tparam TNode The BST node type (automatically deduced as const or
 * non-const).
 * @tparam F The type of the function to apply to each node.
 * @param root The root of the tree to traverse.
 * @param f Function to apply to each node (defaults to returning node->data).
 * @return Vector containing the results of applying f to each node in inorder
 * order.
 */
template <bool apply_deferred = true, typename TNode,
          typename F = decltype([](TNode* n) { return n->data; })>
[[nodiscard]] constexpr std::vector<
    typename std::invoke_result<F, TNode*>::type>
to_vector(TNode* root, F&& f = {}) {
  std::vector<typename std::invoke_result<F, TNode*>::type> output;

  if constexpr (TNode::SubtreeDataType::has_size)
    output.reserve(bst::subtree_data::size(root));

  collect_inorder<apply_deferred>(root, output, std::forward<F>(f));

  return output;
}

}  // namespace base
}  // namespace bst
