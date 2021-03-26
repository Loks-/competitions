#pragma once

#include "common/binary_search_tree/ext/info/helpers/remove_node_to_root.h"
#include "common/binary_search_tree/info/update_node_to_root.h"
#include "common/template.h"

namespace bst {
namespace ext {
namespace info {
namespace hidden {
template <class TNode>
inline void RemoveOrUpdateNodeToRootI(TNode* node, TNode* rem, TFakeFalse) {
  bst::info::UpdateNodeToRoot(node);
}

template <class TNode>
inline void RemoveOrUpdateNodeToRootI(TNode* node, TNode* rem, TFakeTrue) {
  RemoveNodeToRoot(node, rem);
}
}  // namespace hidden

template <class TNode>
inline void RemoveOrUpdateNodeToRoot(TNode* node, TNode* rem) {
  hidden::RemoveOrUpdateNodeToRootI(node, rem,
                                    TFakeBool<TNode::support_remove>());
}
}  // namespace info
}  // namespace ext
}  // namespace bst
