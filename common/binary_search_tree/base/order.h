#pragma once

#include "common/base.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"
#include "common/binary_search_tree/subtree_data/size.h"

namespace bst {
namespace base {
template <class TNode>
size_t Order(TNode* node) {
  static_assert(TNode::use_parent, "use_parent should be true");
  static_assert(TNode::TInfo::has_size, "info should contain size");
  assert(node);
  bst::deferred::propagate_for_structure_access(node);
  size_t order = bst::subtree_data::size(node->l);
  for (; node->p; node = node->p) {
    if (node->p->r == node) {
      order += bst::subtree_data::size(node->p->l) + 1;
    }
  }
  return order;
}
}  // namespace base
}  // namespace bst
