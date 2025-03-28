#pragma once

#include "common/binary_search_tree/base/node.h"

namespace bst {
namespace base {
template <class TNode>
inline bool IsLeaf(TNode* node) {
  if (TNode::DeferredType::modify_tree) node->apply_deferred();
  return (node->left == nullptr) && (node->right == nullptr);
}
}  // namespace base
}  // namespace bst
