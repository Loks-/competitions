#include "common/binary_search_tree/utils/swap_intervals.h"

namespace bst {
// Rotate elements in interval [first, last) to right by shift
template <class TTree, class TNode>
inline void IntervalRotateRight(TNode*& root, size_t first, size_t last,
                                size_t shift) {
  assert(last > first);
  shift %= (last - first);
  if (shift != 0) utils::swap_intervals<TTree>(root, first, last - shift, last);
}

// Rotate elements in interval [first, last) to left by shift
template <class TTree, class TNode>
inline void IntervalRotateLeft(TNode*& root, size_t first, size_t last,
                               size_t shift) {
  assert(last > first);
  shift %= (last - first);
  if (shift != 0)
    utils::swap_intervals<TTree>(root, first, first + shift, last);
}

// Rotate all elements to right by shift
template <class TTree, class TNode>
inline void VectorRotateRight(TNode*& root, size_t shift) {
  if (root)
    IntervalRotateRight<TTree>(root, 0, subtree_data::size(root), shift);
}

// Rotate all elements to left by shift
template <class TTree, class TNode>
inline void VectorRotateLeft(TNode*& root, size_t shift) {
  if (root) IntervalRotateLeft<TTree>(root, 0, subtree_data::size(root), shift);
}
}  // namespace bst
