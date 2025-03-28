#pragma once

#include "common/base.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace base {
template <class TNode>
size_t Order(TNode* node) {
  static_assert(TNode::has_parent, "has_parent should be true");
  static_assert(TNode::SubtreeDataType::has_size,
                "subtree data should contain size");
  assert(node);
  bst::deferred::propagate_for_tree_structure_access(node);
  size_t order = bst::subtree_data::size(node->left);
  for (; node->parent; node = node->parent) {
    if (node->parent->right == node) {
      order += bst::subtree_data::size(node->parent->left) + 1;
    }
  }
  return order;
}
}  // namespace base
}  // namespace bst
