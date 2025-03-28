#pragma once

namespace bst {
namespace base {
template <class TNode>
inline TNode* FindByKey(TNode* root, const typename TNode::KeyType& key) {
  static_assert(TNode::has_key, "has_key should be true");
  for (TNode* node = root; node;) {
    node->apply_deferred();
    if (node->key < key) {
      node = node->right;
    } else if (key < node->key) {
      node = node->left;
    } else {
      return node;
    }
  }
  return nullptr;
}
}  // namespace base
}  // namespace bst
