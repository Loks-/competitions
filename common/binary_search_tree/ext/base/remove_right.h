#pragma once

#include "common/base.h"
#include "common/binary_search_tree/ext/base/remove_right__default.h"
#include "common/binary_search_tree/ext/base/remove_right__skip_update.h"
#include "common/binary_search_tree/ext/base/remove_right__use_parent.h"

namespace bst {
namespace ext {
namespace base {
namespace hidden {
template <class TNode>
inline void RemoveRightI(TNode* root, TNode*& removed_node, TNode*& old_parent,
                         TMetaFalse, TMetaFalse) {
  RemoveRightDefault(root, removed_node, old_parent);
}

template <class TNode>
inline void RemoveRightI(TNode* root, TNode*& removed_node, TNode*& old_parent,
                         TMetaFalse, TMetaTrue) {
  RemoveRightUseParent(root, removed_node, old_parent);
}

template <class TNode>
inline void RemoveRightI(TNode* root, TNode*& removed_node, TNode*& old_parent,
                         TMetaTrue, TMetaFalse) {
  RemoveRightSkipUpdate(root, removed_node, old_parent);
}

template <class TNode>
inline void RemoveRightI(TNode* root, TNode*& removed_node, TNode*& old_parent,
                         TMetaTrue, TMetaTrue) {
  RemoveRightSkipUpdate(root, removed_node, old_parent);
}
}  // namespace hidden

template <class TNode>
inline TNode* RemoveRight(TNode* root, TNode*& removed_node,
                          TNode*& old_parent) {
  assert(root);
  root->ApplyAction();
  if (root->r) {
    hidden::RemoveRightI(root, removed_node, old_parent,
                         TMetaBool<TNode::TInfo::is_none>(),
                         TMetaBool<TNode::use_parent>());
    return root;
  } else {
    removed_node = root;
    old_parent = nullptr;
    TNode* node = root->l;
    if (node) node->SetP(nullptr);
    root->ResetLinksAndUpdateInfo();
    return node;
  }
}
}  // namespace base
}  // namespace ext
}  // namespace bst
