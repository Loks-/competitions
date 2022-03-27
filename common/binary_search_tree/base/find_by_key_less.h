#pragma once

namespace bst {
namespace base {
template <class TNode>
// Find node with biggest key less or equal to 'key'.
inline TNode* FindByKeyLess(TNode* root, const typename TNode::TKey& key) {
  static_assert(TNode::use_key, "use_key should be true");
  TNode* last_less = nullptr;
  for (TNode* node = root; node;) {
    node->ApplyAction();
    if (node->key < key) {
      last_less = node;
      node = node->r;
    } else if (key < node->key) {
      node = node->l;
    } else {
      return node;
    }
  }
  return last_less;
}
}  // namespace base
}  // namespace bst
