#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/swap.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"

namespace bst {
namespace base {
template <class TNode, bool apply_action_to_node>
inline void RemovePushDown(TNode* node) {
  if (apply_action_to_node) bst::deferred::propagate_to_node(node);
  if (!node->left || !node->right) return;
  TNode* swap_node = node->left;
  for (swap_node->apply_deferred(); swap_node->right;
       swap_node->apply_deferred())
    swap_node = swap_node->right;
  swap_nodes(node, node->parent, swap_node, swap_node->parent);
}
}  // namespace base
}  // namespace bst
