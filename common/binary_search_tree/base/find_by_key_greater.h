#pragma once

namespace bst {
namespace base {
template <class TNode>
// Find node with lowest key greater or equal to 'key'.
inline TNode* FindByKeyGreater(TNode* root,
                               const typename TNode::KeyType& key) {
  static_assert(TNode::has_key, "has_key should be true");
  TNode* last_greater = nullptr;
  for (TNode* node = root; node;) {
    node->apply_deferred();
    if (node->key < key) {
      node = node->right;
    } else if (key < node->key) {
      last_greater = node;
      node = node->left;
    } else {
      return node;
    }
  }
  return last_greater;
}
}  // namespace base
}  // namespace bst
