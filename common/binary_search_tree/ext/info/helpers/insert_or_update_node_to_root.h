#pragma once

#include "common/binary_search_tree/ext/info/helpers/insert_node_to_root.h"
#include "common/binary_search_tree/subtree_data/utils/update_node_to_root.h"
#include "common/template.h"

namespace bst {
namespace ext {
namespace info {
namespace hidden {
template <class TNode>
inline void InsertOrUpdateNodeToRootI(TNode* node, TNode* ins, MetaFalse) {
  bst::subtree_data::update_node_to_root(node);
}

template <class TNode>
inline void InsertOrUpdateNodeToRootI(TNode* node, TNode* ins, MetaTrue) {
  InsertNodeToRoot(node, ins);
}
}  // namespace hidden

template <class TNode>
inline void InsertOrUpdateNodeToRoot(TNode* node, TNode* ins) {
  hidden::InsertOrUpdateNodeToRootI(node, ins,
                                    MetaBool<TNode::support_insert>());
}
}  // namespace info
}  // namespace ext
}  // namespace bst
