#pragma once

#include "common/binary_search_tree/ext/base/remove_right__skip_update.h"

namespace bst {
namespace ext {
namespace base {
template <class TNode, class TKey>
inline TNode* RemoveByKeySkipUpdate(TNode* root, const TKey& key,
                                    TNode*& removed_node,
                                    TNode*& first_changed_node) {
  TNode *p = nullptr, *node = root, *c;
  for (;;) {
    node->ApplyAction();
    if (node->key < key) {
      if (!node->r) return root;
      p = node;
      node = node->r;
    } else if (key < node->key) {
      if (!node->l) return root;
      p = node;
      node = node->l;
    } else {
      break;
    }
  }
  removed_node = node;
  if (node->l && node->r) {
    TNode* l = RemoveRightSkipUpdate<TNode>(node->l, c, first_changed_node);
    (p ? ((node == p->l) ? p->l : p->r) : root) = c;
    c->SetL(l);
    c->SetR(node->r);
    c->SetP(p);
  } else {
    first_changed_node = p;
    c = (node->l ? node->l : node->r);
    (p ? ((node == p->l) ? p->l : p->r) : root) = c;
    if (c) c->SetP(p);
  }
  node->ResetLinksAndUpdateInfo();
  return root;
}
}  // namespace base
}  // namespace ext
}  // namespace bst
