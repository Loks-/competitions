#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* FindByOrder(TNode* root, size_t order_index) {
  static_assert(TNode::SubtreeDataType::has_size,
                "subtree data should contain size");
  if (!root) return nullptr;
  if (order_index >= bst::subtree_data::size(root)) return nullptr;
  for (TNode* node = root; node;) {
    node->apply_deferred();
    size_t ls = bst::subtree_data::size(node->left);
    if (order_index < ls) {
      node = node->left;
    } else if (order_index == ls) {
      return node;
    } else {
      order_index -= 1 + ls;
      node = node->right;
    }
  }
  assert(false);
  return nullptr;
}
}  // namespace base
}  // namespace bst
