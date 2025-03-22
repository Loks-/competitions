#pragma once

#include "common/binary_search_tree/ext/base/insert_by_key__default.h"
#include "common/binary_search_tree/ext/base/insert_by_key__use_insert.h"
#include "common/binary_search_tree/ext/base/insert_by_key__use_parent.h"
#include "common/template.h"

namespace bst {
namespace ext {
namespace base {
namespace hidden {
template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, MetaFalse, MetaFalse) {
  InsertByKeyDefault<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, MetaFalse, MetaTrue) {
  InsertByKeyUseParent<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, MetaTrue, MetaFalse) {
  InsertByKeyUseInsert<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, MetaTrue, MetaTrue) {
  InsertByKeyUseInsert<TNode>(root, node);
}
}  // namespace hidden

template <class TNode>
inline void InsertByKey(TNode* root, TNode* node) {
  static_assert(TNode::use_key, "use_key should be true");
  if (!root || !node) return;
  hidden::InsertByKeyI<TNode>(root, node, MetaBool<TNode::support_insert>(),
                              MetaBool<TNode::use_parent>());
}
}  // namespace base
}  // namespace ext
}  // namespace bst
