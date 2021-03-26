#pragma once

#include "common/binary_search_tree/ext/base/remove_right__default.h"

#include <stack>

namespace bst {
namespace ext {
namespace base {
template <class TNode, class TKey>
inline TNode* RemoveByKeySkipUpdate(TNode* root, const TKey& key,
                                    TNode*& removed_node,
                                    TNode*& first_changed_node) {
  thread_local std::stack<TNode*> s;
  TNode *node = root, *c;
  for (;;) {
    s.push(node);
    node->ApplyAction();
    if (node->key < key) {
      if (!node->r) return root;
      node = node->r;
    } else if (key < node->key) {
      if (!node->l) return root;
      node = node->l;
    } else {
      s.pop();
      break;
    }
  }
  removed_node = node;
  TNode* p = s.empty() ? nullptr : s.top();
  if (node->l && node->r) {
    TNode* l = RemoveRightDefault<TNode>(node->l, c, first_changed_node);
    (p ? ((node == p->l) ? p->l : p->r) : root) = c;
    c->SetL(l);
    c->SetR(node->r);
    c->SetP(p);
    c->UpdateInfo();
  } else {
    first_changed_node = p;
    c = (node->l ? node->l : node->r);
    (p ? ((node == p->l) ? p->l : p->r) : root) = c;
    if (c) c->SetP(p);
  }
  node->ResetLinksAndUpdateInfo();
  for (; !s.empty(); s.pop()) s.top()->RemoveOrUpdateInfo(node);
  return root;
}
}  // namespace base
}  // namespace ext
}  // namespace bst
