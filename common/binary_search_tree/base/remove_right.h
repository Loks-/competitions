#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/helpers/remove_or_update_node_to_root.h"
#include "common/template.h"

#include <stack>

namespace bst {
template <class TNode>
inline TNode* RemoveRightSkipUpdate(TNode* root) {
  assert(root && root->r);
  root->ApplyAction();
  for (root->r->ApplyAction(); root->r->r; root->r->ApplyAction()) {
    root = root->r;
  }
  TNode* node = root->r;
  root->SetR(node->l);
  node->ResetLinksAndUpdateInfo();
  return node;
}

template <class TNode>
inline TNode* RemoveRightUseParent(TNode* root) {
  assert(root && root->r);
  TNode* node = root;
  node->ApplyAction();
  for (node->r->ApplyAction(); node->r->r; node->r->ApplyAction()) {
    node = node->r;
  }
  TNode* removed_node = node->r;
  node->SetR(removed_node->l);
  removed_node->ResetLinksAndUpdateInfo();
  for (; node != root->p; node = node->p)
    node->RemoveOrUpdateInfo(removed_node);
  return removed_node;
}

template <class TNode>
inline TNode* RemoveRightDefault(TNode* root) {
  thread_local std::stack<TNode*> s;
  assert(root && root->r);
  root->ApplyAction();
  for (root->r->ApplyAction(); root->r->r; root->r->ApplyAction()) {
    s.push(root);
    root = root->r;
  }
  TNode* node = root->r;
  root->SetR(node->l);
  node->ResetLinksAndUpdateInfo();
  root->UpdateInfo();
  for (; !s.empty(); s.pop()) s.top()->RemoveOrUpdateInfo(node);
  return node;
}

namespace hidden {
template <class TNode>
inline TNode* RemoveRightI(TNode* root, TFakeFalse, TFakeFalse) {
  return RemoveRightDefault(root);
}

template <class TNode>
inline TNode* RemoveRightI(TNode* root, TFakeFalse, TFakeTrue) {
  return RemoveRightUseParent(root);
}

template <class TNode>
inline TNode* RemoveRightI(TNode* root, TFakeTrue, TFakeFalse) {
  return RemoveRightSkipUpdate(root);
}

template <class TNode>
inline TNode* RemoveRightI(TNode* root, TFakeTrue, TFakeTrue) {
  return RemoveRightSkipUpdate(root);
}
}  // namespace hidden

template <class TNode>
inline TNode* RemoveRight(TNode* root, TNode*& removed_node) {
  assert(root);
  if (root->r) {
    removed_node =
        hidden::RemoveRightI(root, TFakeBool<TNode::TInfo::is_none>(),
                             TFakeBool<TNode::use_parent>());
    return root;
  } else {
    removed_node = root;
    TNode* node = root->l;
    if (node) node->SetP(nullptr);
    root->ResetLinksAndUpdateInfo();
    return node;
  }
}
}  // namespace bst
