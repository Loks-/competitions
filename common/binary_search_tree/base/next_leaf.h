#pragma once

#include "common/binary_search_tree/base/leaf.h"
#include "common/binary_search_tree/base/next.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* NextLeaf(TNode* node) {
  for (node = Next(node); node && !IsLeaf(node);) node = Next(node);
  return node;
}
}  // namespace base
}  // namespace bst
