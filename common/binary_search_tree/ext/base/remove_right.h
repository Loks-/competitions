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
                         MetaFalse, MetaFalse) {
  RemoveRightDefault(root, removed_node, old_parent);
}

template <class TNode>
inline void RemoveRightI(TNode* root, TNode*& removed_node, TNode*& old_parent,
                         MetaFalse, MetaTrue) {
  RemoveRightUseParent(root, removed_node, old_parent);
}

template <class TNode>
inline void RemoveRightI(TNode* root, TNode*& removed_node, TNode*& old_parent,
                         MetaTrue, MetaFalse) {
  RemoveRightSkipUpdate(root, removed_node, old_parent);
}

template <class TNode>
inline void RemoveRightI(TNode* root, TNode*& removed_node, TNode*& old_parent,
                         MetaTrue, MetaTrue) {
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
                         MetaBool<TNode::TInfo::is_none>(),
                         MetaBool<TNode::use_parent>());
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
