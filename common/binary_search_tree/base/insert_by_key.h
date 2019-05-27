#pragma once

#include "common/binary_search_tree/info/update_info.h"

namespace bst {
template <class TNode>
inline void InsertByKey(TNode* root, TNode* node) {
  static_assert(TNode::use_key, "use_key should be true");
  static_assert(TNode::use_parent, "use_parent should be true");
  if (!root || !node) return;
  for (;;) {
    root->ApplyAction();
    if (root->key < node->key) {
      if (root->r) {
        root = root->r;
      } else {
        root->r = node;
        break;
      }
    } else {
      if (root->l) {
        root = root->l;
      } else {
        root->l = node;
        break;
      }
    }
  }
  node->p = root;
  UpdateInfoNodeToRoot(root);
}
}  // namespace bst
