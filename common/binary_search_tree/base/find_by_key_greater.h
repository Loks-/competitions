#pragma once

namespace bst {
namespace base {
template <class TNode>
// Find node with lowest key greater or equal to 'key'.
inline TNode* FindByKeyGreater(TNode* root, const typename TNode::TKey& key) {
  static_assert(TNode::use_key, "use_key should be true");
  TNode* last_greater = nullptr;
  for (TNode* node = root; node;) {
    node->ApplyAction();
    if (node->key < key) {
      node = node->r;
    } else if (key < node->key) {
      last_greater = node;
      node = node->l;
    } else {
      return node;
    }
  }
  return last_greater;
}
}  // namespace base
}  // namespace bst
