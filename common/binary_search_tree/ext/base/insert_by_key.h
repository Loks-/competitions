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
inline void InsertByKeyI(TNode* root, TNode* node, TFakeFalse, TFakeFalse) {
  InsertByKeyDefault<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TFakeFalse, TFakeTrue) {
  InsertByKeyUseParent<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TFakeTrue, TFakeFalse) {
  InsertByKeyUseInsert<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TFakeTrue, TFakeTrue) {
  InsertByKeyUseInsert<TNode>(root, node);
}
}  // namespace hidden

template <class TNode>
inline void InsertByKey(TNode* root, TNode* node) {
  static_assert(TNode::use_key, "use_key should be true");
  if (!root || !node) return;
  hidden::InsertByKeyI<TNode>(root, node, TFakeBool<TNode::support_insert>(),
                              TFakeBool<TNode::use_parent>());
}
}  // namespace base
}  // namespace ext
}  // namespace bst
