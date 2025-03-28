#pragma once

namespace bst {
namespace base {
template <class TNode>
constexpr TNode* Sibling(TNode* node, TNode* parent) {
  return parent ? (parent->left == node ? parent->right : parent->left)
                : nullptr;
}
}  // namespace base
}  // namespace bst
