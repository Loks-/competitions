#pragma once

namespace bst {
namespace base {
template <class TNode>
// Find node with biggest key less or equal to 'key'.
inline TNode* FindByKeyLess(TNode* root, const typename TNode::KeyType& key) {
  static_assert(TNode::has_key, "has_key should be true");
  TNode* last_less = nullptr;
  for (TNode* node = root; node;) {
    node->apply_deferred();
    if (node->key < key) {
      last_less = node;
      node = node->right;
    } else if (key < node->key) {
      node = node->left;
    } else {
      return node;
    }
  }
  return last_less;
}
}  // namespace base
}  // namespace bst
