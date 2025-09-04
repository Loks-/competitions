#pragma once

#include "common/base.h"

namespace bst {
namespace utils {

/**
 * @brief Gets segment data for nodes in an index-based range [begin, end) using
 * tree splitting.
 *
 * This function extracts subtree data for nodes in the index range [begin, end)
 * by temporarily splitting the tree, extracting the data from the middle
 * segment, and then rejoining the tree. This approach is useful when you need
 * the aggregated subtree data for a specific range of nodes in inorder
 * traversal.
 *
 * The function works by:
 * 1. Splitting the tree at the end index to separate nodes [0, end) from [end,
 * n)
 * 2. Splitting the left part at the begin index to get nodes [begin, end)
 * 3. Extracting the subtree data from the middle segment
 * 4. Rejoining all parts to restore the original tree structure
 *
 * Note: This function modifies the tree structure temporarily but restores it
 * before returning. The tree remains balanced after the operation.
 *
 * @tparam TTree The BST tree type that provides split_at and join operations.
 * @tparam TNode The BST node type.
 * @param root The root of the tree to extract segment data from (passed by
 * reference).
 * @param begin The begin index (inclusive) for the range.
 * @param end The end index (exclusive) for the range.
 * @return The extracted subtree data for nodes in index range [begin, end).
 * @pre begin <= end
 * @pre The tree's subtree data must contain the required aggregators.
 */
template <class TTree, class TNode>
[[nodiscard]] constexpr typename TTree::SubtreeDataType
get_segment_data_by_index(TNode*& root, size_t begin, size_t end) {
  TNode *l, *m, *r;
  TTree::split_at(root, end, m, r);
  TTree::split_at(m, begin, l, m);
  typename TTree::SubtreeDataType output;
  if (m) output = m->subtree_data;
  root = TTree::join(TTree::join(l, m), r);
  return output;
}

}  // namespace utils
}  // namespace bst
