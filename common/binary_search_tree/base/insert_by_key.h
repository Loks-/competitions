#pragma once

#include "common/binary_search_tree/info/update_node_to_root.h"

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
  bst::info::UpdateNodeToRoot(root);
}
}  // namespace base
}  // namespace bst
