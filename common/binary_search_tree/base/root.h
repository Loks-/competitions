#pragma once

namespace bst {
namespace base {
template <class TNode>
constexpr TNode* Root(TNode* node) {
  static_assert(TNode::has_parent, "has_parent should be true");
  if (!node) return node;
  for (; node->parent;) node = node->parent;
  return node;
}
}  // namespace base
}  // namespace bst
