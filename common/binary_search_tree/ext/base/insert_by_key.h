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
inline void InsertByKeyI(TNode* root, TNode* node, TMetaFalse, TMetaFalse) {
  InsertByKeyDefault<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TMetaFalse, TMetaTrue) {
  InsertByKeyUseParent<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TMetaTrue, TMetaFalse) {
  InsertByKeyUseInsert<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TMetaTrue, TMetaTrue) {
  InsertByKeyUseInsert<TNode>(root, node);
}
}  // namespace hidden

template <class TNode>
inline void InsertByKey(TNode* root, TNode* node) {
  static_assert(TNode::use_key, "use_key should be true");
  if (!root || !node) return;
  hidden::InsertByKeyI<TNode>(root, node, TMetaBool<TNode::support_insert>(),
                              TMetaBool<TNode::use_parent>());
}
}  // namespace base
}  // namespace ext
}  // namespace bst
