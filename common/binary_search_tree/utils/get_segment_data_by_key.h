#pragma once

#include "common/base.h"

namespace bst {
namespace utils {

/**
 * @brief Gets segment data for all nodes with keys less than the given end key
 * using tree splitting.
 *
 * This function extracts subtree data for all nodes with keys less than the
 * given end key by temporarily splitting the tree, extracting the data from
 * the left segment, and then rejoining the tree. This approach is useful when
 * you need the aggregated subtree data for nodes from the beginning of the
 * tree up to a specific key.
 *
 * The function works by:
 * 1. Splitting the tree at the end key to separate nodes with keys < end from
 * >= end
 * 2. Extracting the subtree data from the left segment
 * 3. Rejoining the parts to restore the original tree structure
 *
 * Note: This function modifies the tree structure temporarily but restores it
 * before returning. The tree remains balanced after the operation.
 *
 * @tparam TTree The BST tree type that provides split and join operations.
 * @tparam TNode The BST node type.
 * @param root The root of the tree to extract segment data from (passed by
 * reference).
 * @param end The end key (exclusive) for the range.
 * @return The extracted subtree data for nodes with keys less than end.
 * @pre The tree's subtree data must contain the required aggregators.
 */
template <class TTree, class TNode>
[[nodiscard]] constexpr typename TTree::SubtreeDataType get_segment_data_by_key(
    TNode*& root, const typename TTree::KeyType& end) {
  TNode *l, *r;
  TTree::split(root, end, l, r);
  typename TTree::SubtreeDataType output;
  if (l) output = l->subtree_data;
  root = TTree::join(l, r);
  return output;
}

/**
 * @brief Gets segment data for nodes in a key-based range [begin, end) using
 * tree splitting.
 *
 * This function extracts subtree data for nodes in the key range [begin, end)
 * by temporarily splitting the tree, extracting the data from the middle
 * segment, and then rejoining the tree. This approach is useful when you need
 * the aggregated subtree data for a specific range of keys in the tree.
 *
 * The function works by:
 * 1. Splitting the tree at the end key to separate nodes with keys < end from
 * >= end
 * 2. Splitting the left part at the begin key to get nodes with keys in [begin,
 * end)
 * 3. Extracting the subtree data from the middle segment
 * 4. Rejoining all parts to restore the original tree structure
 *
 * Note: This function modifies the tree structure temporarily but restores it
 * before returning. The tree remains balanced after the operation.
 *
 * @tparam TTree The BST tree type that provides split and join operations.
 * @tparam TNode The BST node type.
 * @param root The root of the tree to extract segment data from (passed by
 * reference).
 * @param begin The begin key (inclusive) for the range.
 * @param end The end key (exclusive) for the range.
 * @return The extracted subtree data for nodes in key range [begin, end).
 * @pre begin < end (keys must be comparable)
 * @pre The tree's subtree data must contain the required aggregators.
 */
template <class TTree, class TNode>
[[nodiscard]] constexpr typename TTree::SubtreeDataType get_segment_data_by_key(
    TNode*& root, const typename TTree::KeyType& begin,
    const typename TTree::KeyType& end) {
  TNode *l, *m, *r;
  TTree::split(root, end, m, r);
  TTree::split(m, begin, l, m);
  typename TTree::SubtreeDataType output;
  if (m) output = m->subtree_data;
  root = TTree::join(TTree::join(l, m), r);
  return output;
}

}  // namespace utils
}  // namespace bst
