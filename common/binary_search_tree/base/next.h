#pragma once

#include "common/binary_search_tree/base/left.h"
#include "common/binary_search_tree/base/node.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* Next(TNode* node) {
  if (!node) return nullptr;
  if (TNode::TAction::modify_tree) node->ApplyAction();
  if (node->r) return Left(node->r);
  for (; node->p && (node->p->r == node);) node = node->p;
  return node->p;
}
}  // namespace base
}  // namespace bst
