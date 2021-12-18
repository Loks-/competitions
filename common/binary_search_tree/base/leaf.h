#pragma once

namespace bst {
namespace base {
template <class TNode>
inline bool IsLeaf(TNode* node) {
  return (node->l == nullptr) && (node->r == nullptr);
}
}  // namespace base
}  // namespace bst
