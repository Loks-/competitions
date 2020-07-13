#pragma once

#include "common/binary_search_tree/base/remove_right.h"
#include "common/binary_search_tree/info/helpers/remove_or_update_node_to_root.h"
#include "common/template.h"

#include <stack>

namespace bst {
template <class TNode, class TKey>
inline TNode* RemoveByKeySkipUpdate(TNode* root, const TKey& key,
                                    TNode*& removed_node) {
  TNode *p = nullptr, *node = root;
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
    TNode* t;
    TNode* l = RemoveRight<TNode>(node->l, t);
    (p ? ((node == p->l) ? p->l : p->r) : root) = t;
    t->SetL(l);
    t->SetR(node->r);
    t->SetP(p);
    t->UpdateInfo();
  } else {
    TNode* c = (node->l ? node->l : node->r);
    (p ? ((node == p->l) ? p->l : p->r) : root) = c;
    if (c) c->SetP(p);
  }
  node->ResetLinksAndUpdateInfo();
  return root;
}

template <class TNode, class TKey>
inline TNode* RemoveByKeyUseParent(TNode* root, const TKey& key,
                                   TNode*& removed_node) {
  TNode* node = root;
  for (;;) {
    node->ApplyAction();
    if (node->key < key) {
      if (!node->r) return root;
      node = node->r;
    } else if (key < node->key) {
      if (!node->l) return root;
      node = node->l;
    } else {
      break;
    }
  }
  removed_node = node;
  TNode* p = node->p;
  if (node->l && node->r) {
    TNode* t;
    TNode* l = RemoveRight<TNode>(node->l, t);
    (p ? ((node == p->l) ? p->l : p->r) : root) = t;
    t->SetL(l);
    t->SetR(node->r);
    t->SetP(p);
    t->UpdateInfo();
  } else {
    TNode* c = (node->l ? node->l : node->r);
    (p ? ((node == p->l) ? p->l : p->r) : root) = c;
    if (c) c->SetP(p);
  }
  node->ResetLinksAndUpdateInfo();
  info::RemoveOrUpdateNodeToRoot(p, node);
  return root;
}

template <class TNode, class TKey>
inline TNode* RemoveByKeyDefault(TNode* root, const TKey& key,
                                 TNode*& removed_node) {
  static_assert(!TNode::use_parent, "Parents links are not supported");
  thread_local std::stack<TNode*> s;
  TNode* node = root;
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
    TNode* t;
    TNode* l = RemoveRight<TNode>(node->l, t);
    (p ? ((node == p->l) ? p->l : p->r) : root) = t;
    t->l = l;
    t->r = node->r;
    t->UpdateInfo();
  } else {
    (p ? ((node == p->l) ? p->l : p->r) : root) = (node->l ? node->l : node->r);
  }
  node->ResetLinksAndUpdateInfo();
  for (; !s.empty(); s.pop()) s.top()->RemoveOrUpdateInfo(node);
  return root;
}

namespace hidden {
template <class TNode, class TKey>
inline TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                           TFakeFalse, TFakeFalse) {
  return RemoveByKeyDefault(root, key, removed_node);
}

template <class TNode, class TKey>
inline TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                           TFakeFalse, TFakeTrue) {
  return RemoveByKeyUseParent(root, key, removed_node);
}

template <class TNode, class TKey>
inline TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                           TFakeTrue, TFakeFalse) {
  return RemoveByKeySkipUpdate(root, key, removed_node);
}

template <class TNode, class TKey>
inline TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                           TFakeTrue, TFakeTrue) {
  return RemoveByKeySkipUpdate(root, key, removed_node);
}
}  // namespace hidden

template <class TNode, class TKey>
inline TNode* RemoveByKey(TNode* root, const TKey& key, TNode*& removed_node) {
  static_assert(TNode::use_key, "use_key should be true");
  if (!root) return root;
  return hidden::RemoveByKeyI(root, key, removed_node,
                              TFakeBool<TNode::TInfo::is_none>(),
                              TFakeBool<TNode::use_parent>());
}
}  // namespace bst
