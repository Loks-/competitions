#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_root_to_node.h"

namespace bst {
namespace base {
template <class TNode>
size_t Order(TNode* node) {
  static_assert(TNode::use_parent, "use_parent should be true");
  static_assert(TNode::TInfo::has_size, "info should contain size");
  assert(node);
  bst::action::ApplyRootToNode_ModifyTree(node);
  size_t order = 0;
  if (node->l) order += node->l->subtree_data.size;
  for (; node->p; node = node->p) {
    if (node->p->r == node) {
      if (node->p->l) order += node->p->l->subtree_data.size;
      ++order;
    }
  }
  return order;
}
}  // namespace base
}  // namespace bst
