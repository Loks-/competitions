#pragma once

#include "common/binary_search_tree/base/left.h"
#include "common/binary_search_tree/base/node.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* Next(TNode* node) {
  if (!node) return nullptr;
  if (TNode::DeferredType::modify_tree) node->apply_deferred();
  if (node->right) return Left(node->right);
  for (; node->parent && (node->parent->right == node);) node = node->parent;
  return node->parent;
}
}  // namespace base
}  // namespace bst
