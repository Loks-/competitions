#pragma once

namespace bst {
namespace base {
template <class TNode>
constexpr TNode* Sibling(TNode* node, TNode* parent) {
  return parent ? (parent->l == node ? parent->r : parent->l) : nullptr;
}
}  // namespace base
}  // namespace bst
