#pragma once

#include "common/binary_search_tree/subtree_data/utils/update_node_to_root.h"

namespace bst {
namespace base {
template <class TNode>
inline void InsertByKey(TNode* root, TNode* node) {
  for (;;) {
    root->ApplyAction();
    if (root->key < node->key) {
      if (root->r) {
        root = root->r;
      } else {
        root->SetR(node);
        break;
      }
    } else {
      if (root->l) {
        root = root->l;
      } else {
        root->SetL(node);
        break;
      }
    }
  }
  bst::subtree_data::update_node_to_root(root);
}
}  // namespace base
}  // namespace bst
