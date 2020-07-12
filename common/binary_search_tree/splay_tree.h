#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_root_to_node.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/node.h"
#include "common/binary_search_tree/tree.h"
#include "common/nodes_manager_fixed_size.h"

namespace bst {
template <bool _use_key, class TTData, class TTInfo = info::Size,
          class TTAction = action::None, class TTKey = int64_t,
          template <class> class TTNodesManager = NodesManagerFixedSize>
class SplayTree
    : public Tree<TTNodesManager<Node<TTData, TTInfo, TTAction, _use_key, true,
                                      false, TTKey>>,
                  SplayTree<_use_key, TTData, TTInfo, TTAction, TTKey,
                            TTNodesManager>> {
 public:
  static const bool use_key = _use_key;
  static const bool use_parent = true;
  static const bool use_height = false;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TKey = TTKey;
  using TNode =
      Node<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
  using TSelf = SplayTree<use_key, TData, TInfo, TAction, TKey, TTNodesManager>;
  using TTree = Tree<TTNodesManager<TNode>, TSelf>;
  friend class Tree<TTNodesManager<TNode>, TSelf>;

 public:
  explicit SplayTree(size_t max_nodes) : TTree(max_nodes) {}
  SplayTree() : SplayTree(0) {}

  // Splay assumes that actions are already applied from root to node.
  static void Splay(TNode* node) {
    if (!node) return;
    for (;;) {
      TNode* parent = node->p;
      if (!parent) break;
      TNode* gparent = parent->p;
      if (!gparent) {
        Rotate<TNode, false, false>(node, parent, gparent);
        break;
      }
      bool zigzig = ((gparent->l == parent) == (parent->l == node));
      RotateUp<TNode, false, false>(zigzig ? parent : node);
      RotateUp<TNode, false, false>(node);
    }
    node->UpdateInfo();
  }

  static TNode* Join(TNode* l, TNode* r) {
    if (!l) return r;
    if (!r) return l;
    assert(!r->p);
    TNode* p = l;
    for (;; p = p->r) {
      p->ApplyAction();
      if (!p->r) break;
    }
    Splay(p);
    p->SetR(r);
    p->UpdateInfo();
    return p;
  }

  static TNode* Join(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->l && !m1->r);
    m1->SetL(l);
    m1->SetR(r);
    return m1;
  }

  // Split tree to 2 trees.
  // p and everything left will go to left tree (and p is root).
  // everything right will go to right tree.
  static TNode* Split(TNode* p) {
    if (!p) return nullptr;
    action::ApplyRootToNode(p);
    Splay(p);
    TNode* r = p->r;
    if (r) {
      r->p = nullptr;
      p->r = nullptr;
      p->UpdateInfo();
    }
    return r;
  }

  static TNode* FindByKey(TNode*& root, const TKey& key) {
    static_assert(use_key, "use_key should be true");
    TNode *node = root, *last_node = nullptr;
    for (; node;) {
      last_node = node;
      node->ApplyAction();
      if (node->key < key)
        node = node->r;
      else if (key < node->key)
        node = node->l;
      else
        break;
    }
    Splay(last_node);
    root = last_node;
    return node;
  }

  static TNode* FindByKey_Less(TNode*& root, const TKey& key) {
    static_assert(use_key, "use_key should be true");
    TNode *last_less = nullptr, *last_node = root;
    for (TNode* node = root; node;) {
      node->ApplyAction();
      if (node->key < key) {
        last_less = node;
        node = node->r;
      } else {
        last_node = node;
        node = node->l;
      }
    }
    root = last_less ? last_less : last_node;
    Splay(root);
    return last_less;
  }

  static void SplitByKey(TNode* root, const TKey& key, TNode*& output_l,
                         TNode*& output_r) {
    static_assert(use_key, "use_key should be true");
    if (!root) {
      output_l = output_r = nullptr;
      return;
    }
    TNode* p = FindByKey_Less(root, key);
    output_l = p;
    output_r = (p ? Split(p) : root);
  }

  static TNode* FindByOrder(TNode*& root, size_t order_index) {
    static_assert(TInfo::has_size, "info should contain size");
    if (!root) return nullptr;
    if (order_index >= root->info.size) return nullptr;
    for (TNode* node = root; node;) {
      node->ApplyAction();
      size_t ls = (node->l ? node->l->info.size : 0);
      if (order_index < ls) {
        node = node->l;
      } else if (order_index == ls) {
        Splay(node);
        root = node;
        return node;
      } else {
        order_index -= 1 + ls;
        node = node->r;
      }
    }
    assert(false);
    return nullptr;
  }

  static void SplitBySize(TNode* root, size_t lsize, TNode*& output_l,
                          TNode*& output_r) {
    static_assert(TInfo::has_size, "info should contain size");
    if (!root) {
      output_l = output_r = nullptr;
    } else if (lsize == 0) {
      output_l = nullptr;
      output_r = root;
    } else if (lsize >= root->info.size) {
      output_l = root;
      output_r = nullptr;
    } else {
      TNode* p = FindByOrder(root, lsize - 1);
      output_l = p;
      output_r = Split(p);
    }
  }

  static TNode* InsertByKey(TNode* root, TNode* node) {
    static_assert(use_key, "use_key should be true");
    assert(node);
    if (!root) return node;
    SplitByKey(root, node->key, node->l, node->r);
    if (node->l) node->l->p = node;
    if (node->r) node->r->p = node;
    node->UpdateInfo();
    return node;
  }

 protected:
  static TNode* RemoveByNodeI(TNode* node) {
    TNode* l = node->l;
    if (l) l->SetP(nullptr);
    TNode* r = node->r;
    if (r) r->SetP(nullptr);
    TNode* p = node->p;
    node->ResetLinksAndUpdateInfo();
    TNode* m = Join(l, r);
    if (!p) return m;
    if (node == p->l)
      p->SetL(m);
    else
      p->SetR(m);
    Splay(p);
    return p;
  }
};
}  // namespace bst
