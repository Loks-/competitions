#pragma once

#include "common/binary_search_tree/base/node.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* Right(TNode* root) {
  if (!root) return nullptr;
  if (TNode::TAction::modify_tree) root->ApplyAction();
  for (; root->r;) {
    root = root->r;
    if (TNode::TAction::modify_tree) root->ApplyAction();
  }
  return root;
}
}  // namespace base
}  // namespace bst
