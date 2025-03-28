#pragma once

#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"

namespace bst {
namespace base {
template <class TNode>
inline void InsertByKey(TNode* root, TNode* node) {
  for (;;) {
    root->apply_deferred();
    if (root->key < node->key) {
      if (root->right) {
        root = root->right;
      } else {
        root->set_right(node);
        break;
      }
    } else {
      if (root->left) {
        root = root->left;
      } else {
        root->set_left(node);
        break;
      }
    }
  }
  bst::subtree_data::propagate_to_root(root);
}
}  // namespace base
}  // namespace bst
