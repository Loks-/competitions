#pragma once

namespace bst {
template <class TNode>
inline TNode* Sibling(TNode* node, TNode* parent) {
  return parent ? (parent->l == node ? parent->r : parent->l) : nullptr;
}
}  // namespace bst
