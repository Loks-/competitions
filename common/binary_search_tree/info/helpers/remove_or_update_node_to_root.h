#pragma once

#include "common/binary_search_tree/info/helpers/remove_node_to_root.h"
#include "common/binary_search_tree/info/helpers/update_node_to_root.h"
#include "common/template.h"

namespace bst {
namespace info {
namespace hidden {
template <class TNode>
inline void RemoveOrUpdateNodeToRootI(TNode* node, TNode* rem, TFakeFalse) {
  UpdateNodeToRoot(node);
}

template <class TNode>
inline void RemoveOrUpdateNodeToRootI(TNode* node, TNode* rem, TFakeTrue) {
  RemoveNodeToRoot(node, rem);
}
}  // namespace hidden

template <class TNode>
inline void RemoveOrUpdateNodeToRoot(TNode* node, TNode* rem) {
  hidden::RemoveOrUpdateNodeToRootI(node, rem,
                                    TFakeBool<TNode::support_insert>());
}
}  // namespace info
}  // namespace bst
