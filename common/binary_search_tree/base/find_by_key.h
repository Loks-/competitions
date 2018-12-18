#pragma once

template <class TNode>
inline TNode* BSTFindByKey(TNode* root, const typename TNode::TKey& key) {
  static_assert(TNode::use_key, "use_key should be true");
  for (TNode* node = root; node != 0;) {
    node->ApplyAction();
    if (node->key < key)
      node = node->r;
    else if (key < node->key)
      node = node->l;
    else
      return node;
  }
  return 0;
}
