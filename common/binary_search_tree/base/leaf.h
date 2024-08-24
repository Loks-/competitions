#pragma once

#include "common/binary_search_tree/base/node.h"

namespace bst {
namespace base {
template <class TNode>
inline bool IsLeaf(TNode* node) {
  if (TNode::TAction::modify_tree) node->ApplyAction();
  return (node->l == nullptr) && (node->r == nullptr);
}
}  // namespace base
}  // namespace bst
