#pragma once

#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"
#include "common/template.h"

namespace bst {
namespace base {
namespace hidden {
template <class TNode>
constexpr unsigned DeepI(const TNode* node) {
  static_assert(TNode::use_parent, "use_parent should be true");
  unsigned d = 0;
  for (; node; node = node->p) ++d;
  return d;
}
}  // namespace hidden

template <class TNode>
inline unsigned Deep(const TNode* node) {
  // bst::action::hidden::ApplyRootToNodeI(
  //     node, MetaBool<TNode::TAction::modify_tree>{});
  bst::deferred::propagate_to_node(node);
  return hidden::DeepI(node);
}
}  // namespace base
}  // namespace bst
