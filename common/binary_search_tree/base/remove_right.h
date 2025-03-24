#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/utils/update_node_to_root.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* RemoveRight(TNode* root, TNode*& removed_node,
                          TNode*& old_parent) {
  assert(root);
  root->ApplyAction();
  if (root->r) {
    TNode* node = root;
    for (node->r->ApplyAction(); node->r->r; node->r->ApplyAction())
      node = node->r;
    removed_node = node->r;
    node->SetR(removed_node->l);
    removed_node->ResetLinksAndUpdateInfo();
    old_parent = node;
    bst::subtree_data::update_node_to_root(old_parent);
    return root;
  } else {
    removed_node = root;
    old_parent = nullptr;
    TNode* node = root->l;
    if (node) node->SetP(nullptr);
    root->ResetLinksAndUpdateInfo();
    return node;
  }
}
}  // namespace base
}  // namespace bst
