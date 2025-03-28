#pragma once

#include "common/binary_search_tree/base/leaf.h"
#include "common/binary_search_tree/base/next.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* NextLeaf(TNode* node) {
  for (node = next(node); node && !leaf(node);) node = next(node);
  return node;
}
}  // namespace base
}  // namespace bst
