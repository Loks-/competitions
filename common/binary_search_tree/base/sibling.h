#pragma once

namespace bst {
namespace base {
template <class TNode>
inline TNode* Sibling(TNode* node, TNode* parent) {
  return parent ? (parent->l == node ? parent->r : parent->l) : nullptr;
}
}  // namespace base
}  // namespace bst
