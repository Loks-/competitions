#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/extended_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"

#include <algorithm>

namespace bst {
template <bool use_key, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t,
          template <class> class TTNodesManager =
              memory::ContiguousNodesManager>
class SplayTree
    : public base::ExtendedTree<
          TTNodesManager<
              base::Node<TData, base::SubtreeData<TAggregatorsTuple>,
                         base::Deferred<TDeferredTuple>, true, use_key, TKey>>,
          SplayTree<use_key, TData, TAggregatorsTuple, TDeferredTuple, TKey,
                    TTNodesManager>> {
 public:
  static constexpr bool support_remove = true;
  static constexpr bool support_join = true;
  static constexpr bool support_join3 = true;
  static constexpr bool support_split = true;

  using TSubtreeData = base::SubtreeData<TAggregatorsTuple>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode = base::Node<TData, TSubtreeData, TDeferred, true, use_key, TKey>;
  using TSelf = SplayTree<use_key, TData, TAggregatorsTuple, TDeferredTuple,
                          TKey, TTNodesManager>;
  using TTree = base::ExtendedTree<TTNodesManager<TNode>, TSelf>;
  friend TTree;

 public:
  explicit SplayTree(size_t max_nodes) : TTree(max_nodes) {}
  SplayTree() : SplayTree(0) {}

  // Splay assumes that actions are already applied from root to node.
  static void Splay(TNode* node) {
    if (!node) return;
    for (;;) {
      TNode* parent = node->parent;
      if (!parent) break;
      TNode* gparent = parent->parent;
      if (!gparent) {
        base::Rotate<false, false, TNode>(node, parent, gparent);
        break;
      }
      bool zigzig = ((gparent->left == parent) == (parent->left == node));
      base::RotateUp<false, false>(zigzig ? parent : node);
      base::RotateUp<false, false>(node);
    }
    node->update_subtree_data();
  }

  static TNode* join(TNode* l, TNode* r) {
    if (!l) return r;
    if (!r) return l;
    assert(!r->parent);
    TNode* p = l;
    for (;; p = p->right) {
      p->apply_deferred();
      if (!p->right) break;
    }
    Splay(p);
    p->set_right(r);
    p->update_subtree_data();
    return p;
  }

  static TNode* join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->left && !m1->right);
    m1->set_left(l);
    m1->set_right(r);
    m1->update_subtree_data();
    return m1;
  }

  // Split tree to 2 trees.
  // p and everything rigth will go to the rigth tree (and p is root).
  // everything left will go to the left tree.
  static TNode* SplitL(TNode* p) {
    if (!p) return nullptr;
    deferred::propagate_to_node(p);
    Splay(p);
    TNode* l = p->left;
    if (l) {
      l->parent = nullptr;
      p->left = nullptr;
      p->update_subtree_data();
    }
    return l;
  }

  // Split tree to 2 trees.
  // p and everything left will go to the left tree (and p is root).
  // everything right will go to the right tree.
  static TNode* SplitR(TNode* p) {
    if (!p) return nullptr;
    deferred::propagate_to_node(p);
    Splay(p);
    TNode* r = p->right;
    if (r) {
      r->parent = nullptr;
      p->right = nullptr;
      p->update_subtree_data();
    }
    return r;
  }

  static TNode* find(TNode*& root, const TKey& key) {
    static_assert(use_key, "use_key should be true");
    TNode *node = root, *last_node = nullptr;
    for (; node;) {
      last_node = node;
      node->apply_deferred();
      if (node->key < key) {
        node = node->right;
      } else if (key < node->key) {
        node = node->left;
      } else {
        break;
      }
    }
    Splay(last_node);
    root = last_node;
    return node;
  }

  static TNode* floor(TNode*& root, const TKey& key) {
    static_assert(use_key, "use_key should be true");
    TNode *last_less = nullptr, *last_node = root;
    for (TNode* node = root; node;) {
      node->apply_deferred();
      if (node->key < key) {
        last_less = node;
        node = node->right;
      } else {
        last_node = node;
        node = node->left;
      }
    }
    root = last_less ? last_less : last_node;
    Splay(root);
    return last_less;
  }

  static void split(TNode* root, const TKey& key, TNode*& output_l,
                    TNode*& output_r) {
    static_assert(use_key, "use_key should be true");
    if (!root) {
      output_l = output_r = nullptr;
      return;
    }
    TNode* p = floor(root, key);
    output_l = p;
    output_r = (p ? SplitR(p) : root);
  }

  static size_t index(TNode* node) {
    Splay(node);
    return subtree_data::size(node->left);
  }

  static TNode* at(TNode*& root, size_t order_index) {
    static_assert(TSubtreeData::has_size, "info should contain size");
    auto node = TTree::at(root, order_index);
    Splay(node);
    if (node) root = node;
    return node;
  }

  static void split_at(TNode* root, size_t lsize, TNode*& output_l,
                       TNode*& output_r) {
    static_assert(TSubtreeData::has_size, "info should contain size");
    if (!root) {
      output_l = output_r = nullptr;
    } else if (lsize == 0) {
      output_l = nullptr;
      output_r = root;
    } else if (lsize >= subtree_data::size(root)) {
      output_l = root;
      output_r = nullptr;
    } else {
      TNode* p = at(root, lsize);
      output_l = SplitL(p);
      output_r = p;
    }
  }

  static TNode* insert(TNode* root, TNode* node) {
    static_assert(use_key, "use_key should be true");
    assert(node);
    if (!root) return node;
    split(root, node->key, node->left, node->right);
    if (node->left) node->left->parent = node;
    if (node->right) node->right->parent = node;
    node->update_subtree_data();
    return node;
  }

  static TNode* Union(TNode* root1, TNode* root2) {
    static_assert(use_key, "use_key should be true");
    static_assert(TSubtreeData::has_size, "info should contain size");
    if (!root1) return root2;
    if (!root2) return root1;
    if (subtree_data::size(root1) < subtree_data::size(root2))
      std::swap(root1, root2);
    TNode* m = at(root1, subtree_data::size(root1) / 2);
    TNode *r2l = nullptr, *r2r = nullptr;
    split(root2, m->key, r2l, r2r);
    if (m->left) m->left->parent = nullptr;
    if (m->right) m->right->parent = nullptr;
    m->set_left(Union(m->left, r2l));
    m->set_right(Union(m->right, r2r));
    m->update_subtree_data();
    return m;
  }

 protected:
  static TNode* remove_node_impl(TNode* node) {
    TNode* l = node->left;
    if (l) l->set_parent(nullptr);
    TNode* r = node->right;
    if (r) r->set_parent(nullptr);
    TNode* p = node->parent;
    node->reset_links_and_update_subtree_data();
    TNode* m = join(l, r);
    if (!p) return m;
    if (node == p->left) {
      p->set_left(m);
    } else {
      p->set_right(m);
    }
    Splay(p);
    return p;
  }
};
}  // namespace bst
