#pragma once

#include "common/binary_search_tree/info/helpers/insert_node_to_root.h"
#include "common/binary_search_tree/info/helpers/update_node_to_root.h"
#include "common/template.h"

namespace bst {
namespace info {
namespace hidden {
template <class TNode>
inline void InsertOrUpdateInfoNodeToRootI(TNode* node, TNode* ins, TFakeFalse) {
  UpdateInfoNodeToRoot(node);
}

template <class TNode>
inline void InsertOrUpdateInfoNodeToRootI(TNode* node, TNode* ins, TFakeTrue) {
  InsertInfoNodeToRoot(node, ins);
}
}  // namespace hidden

template <class TNode>
inline void InsertOrUpdateInfoNodeToRoot(TNode* node, TNode* ins) {
  hidden::InsertOrUpdateInfoNodeToRootI(node, ins,
                                        TFakeBool<TNode::support_insert>());
}
}  // namespace info
}  // namespace bst
