#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/swap.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"

namespace bst {
namespace base {
template <class TNode, bool apply_action_to_node>
inline void RemovePushDown(TNode* node) {
  if (apply_action_to_node) bst::deferred::propagate_to_node(node);
  if (!node->l || !node->r) return;
  TNode* swap_node = node->l;
  for (swap_node->ApplyAction(); swap_node->r; swap_node->ApplyAction())
    swap_node = swap_node->r;
  SwapAuto(node, node->p, swap_node, swap_node->p);
}
}  // namespace base
}  // namespace bst
