#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_root_to_node.h"
#include "common/binary_search_tree/base/swap.h"
#include "common/binary_search_tree/subtree_data/utils/update_node_to_root.h"

namespace bst {
template <class TNode, bool update_info, bool apply_action>
inline void SwapProxy(TNode* node1, TNode* node2) {
  static_assert(TNode::use_parent, "use_parent should be true");
  assert(node1 && node2);
  if (node1 == node2) return;
  if (node1->p == node2) {
    if (apply_action) ApplyActionRootToNode(node1);
    SwapChildParent(node1, node2, node2->p);
    if (update_info) UpdateInfoNodeToRoot(node2);
  } else if (node2->p == node1) {
    if (apply_action) ApplyActionRootToNode(node2);
    SwapChildParent(node2, node1, node1->p);
    if (update_info) UpdateInfoNodeToRoot(node1);
  } else {
    if (apply_action) {
      ApplyActionRootToNode(node1);
      ApplyActionRootToNode(node2);
    }
    SwapNotRelated(node1, node1->p, node2, node2->p);
    if (update_info) {
      UpdateInfoNodeToRoot(node1);
      UpdateInfoNodeToRoot(node2);
    }
  }
}
}  // namespace bst
