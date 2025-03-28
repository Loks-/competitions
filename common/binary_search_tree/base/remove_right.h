#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* RemoveRight(TNode* root, TNode*& removed_node,
                          TNode*& old_parent) {
  assert(root);
  root->apply_deferred();
  if (root->right) {
    TNode* node = root;
    for (node->right->apply_deferred(); node->right->right;
         node->right->apply_deferred())
      node = node->right;
    removed_node = node->right;
    node->set_right(removed_node->left);
    removed_node->reset_links_and_update_subtree_data();
    old_parent = node;
    bst::subtree_data::propagate_to_root(old_parent);
    return root;
  } else {
    removed_node = root;
    old_parent = nullptr;
    TNode* node = root->left;
    if (node) node->set_parent(nullptr);
    root->reset_links_and_update_subtree_data();
    return node;
  }
}
}  // namespace base
}  // namespace bst
