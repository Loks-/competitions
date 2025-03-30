#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
// Swap intervals [first, middle) and [middle, last)
template <class TTree>
inline void SwapIntervals(typename TTree::TNode*& root, size_t first,
                          size_t middle, size_t last) {
  static_assert(!TTree::has_key, "has_key should be false");
  if ((middle <= first) || (middle >= last)) return;
  typename TTree::TNode *p1, *p2, *p3, *p4;
  TTree::split_at(root, middle, p2, p3);
  TTree::split_at(p2, first, p1, p2);
  TTree::split_at(p3, last - middle, p3, p4);
  root = TTree::join(TTree::join(p1, p3), TTree::join(p2, p4));
}

// Rotate elements in interval [first, last) to right by shift
template <class TTree>
inline void IntervalRotateRight(typename TTree::TNode*& root, size_t first,
                                size_t last, size_t shift) {
  assert(last > first);
  shift %= (last - first);
  if (shift != 0) SwapIntervals<TTree>(root, first, last - shift, last);
}

// Rotate elements in interval [first, last) to left by shift
template <class TTree>
inline void IntervalRotateLeft(typename TTree::TNode*& root, size_t first,
                               size_t last, size_t shift) {
  assert(last > first);
  shift %= (last - first);
  if (shift != 0) SwapIntervals<TTree>(root, first, first + shift, last);
}

// Rotate all elements to right by shift
template <class TTree>
inline void VectorRotateRight(typename TTree::TNode*& root, size_t shift) {
  if (root)
    IntervalRotateRight<TTree>(root, 0, subtree_data::size(root), shift);
}

// Rotate all elements to left by shift
template <class TTree>
inline void VectorRotateLeft(typename TTree::TNode*& root, size_t shift) {
  if (root) IntervalRotateLeft<TTree>(root, 0, subtree_data::size(root), shift);
}
}  // namespace bst
