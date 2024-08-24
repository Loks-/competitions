#pragma once

#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/right.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* Prev(TNode* node) {
  if (!node) return nullptr;
  if (TNode::TAction::modify_tree) node->ApplyAction();
  if (node->l) return Right(node->l);
  for (; node->p && (node->p->l == node);) node = node->p;
  return node->p;
}
}  // namespace base
}  // namespace bst
