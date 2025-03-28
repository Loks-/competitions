#pragma once

#include "common/binary_search_tree/base/node.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* Right(TNode* root) {
  if (!root) return nullptr;
  if (TNode::DeferredType::modify_tree) root->apply_deferred();
  for (; root->right;) {
    root = root->right;
    if (TNode::DeferredType::modify_tree) root->apply_deferred();
  }
  return root;
}
}  // namespace base
}  // namespace bst
