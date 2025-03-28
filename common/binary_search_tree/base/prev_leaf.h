#pragma once

#include "common/binary_search_tree/base/leaf.h"
#include "common/binary_search_tree/base/prev.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* PrevLeaf(TNode* node) {
  for (node = prev(node); node && !IsLeaf(node);) node = prev(node);
  return node;
}
}  // namespace base
}  // namespace bst
