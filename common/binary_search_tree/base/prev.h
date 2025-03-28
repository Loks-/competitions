#pragma once

#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/right.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* Prev(TNode* node) {
  if (!node) return nullptr;
  if (TNode::DeferredType::modify_tree_structure) node->apply_deferred();
  if (node->left) return Right(node->left);
  for (; node->parent && (node->parent->left == node);) node = node->parent;
  return node->parent;
}
}  // namespace base
}  // namespace bst
