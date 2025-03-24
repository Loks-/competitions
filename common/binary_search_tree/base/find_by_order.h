#pragma once

#include "common/base.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace base {
template <class TNode>
inline TNode* FindByOrder(TNode* root, size_t order_index) {
  static_assert(TNode::TInfo::has_size, "info should contain size");
  if (!root) return nullptr;
  if (order_index >= bst::subtree_data::size(root)) return nullptr;
  for (TNode* node = root; node;) {
    node->ApplyAction();
    size_t ls = bst::subtree_data::size(node->l);
    if (order_index < ls) {
      node = node->l;
    } else if (order_index == ls) {
      return node;
    } else {
      order_index -= 1 + ls;
      node = node->r;
    }
  }
  assert(false);
  return nullptr;
}
}  // namespace base
}  // namespace bst
