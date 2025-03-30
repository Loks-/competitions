#pragma once
#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/base/tree.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/treap_height.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <algorithm>
#include <stack>
#include <tuple>
#include <vector>

namespace bst {

template <bool use_key, bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t,
          template <class> class TTNodesManager =
              memory::ContiguousNodesManager>
class Treap
    : public base::Tree<
          TTNodesManager<base::Node<
              TData,
              base::SubtreeData<templates::PrependT<subtree_data::TreapHeight,
                                                    TAggregatorsTuple>>,
              base::Deferred<TDeferredTuple>, use_parent, use_key, TKey>>,
          Treap<use_key, use_parent, TData, TAggregatorsTuple, TDeferredTuple,
                TKey, TTNodesManager>> {
 public:
  static constexpr bool support_remove = true;
  static constexpr bool support_join = true;
  static constexpr bool support_join3 = true;
  static constexpr bool support_split = true;

  using TTreapHeight = subtree_data::TreapHeight;
  using TSubtreeData = base::SubtreeData<
      templates::PrependT<subtree_data::TreapHeight, TAggregatorsTuple>>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode =
      base::Node<TData, TSubtreeData, TDeferred, use_parent, use_key, TKey>;
  using TSelf = Treap<use_key, use_parent, TData, TAggregatorsTuple,
                      TDeferredTuple, TKey, TTNodesManager>;
  using TTree = base::Tree<TTNodesManager<TNode>, TSelf>;
  friend TTree;

 protected:
  static constexpr unsigned TreapHeight(const TNode* node) {
    return subtree_data::TreapHeight::get(node);
  }

 public:
  explicit Treap(size_t max_nodes) : TTree(max_nodes) {}

 public:
  static TNode* build_tree(const std::vector<TNode*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    TNode* proot = nodes[0];
    TNode* plast = proot;
    std::stack<TNode*> s;
    for (size_t j = 1; j < nodes.size(); ++j) {
      TNode* pj = nodes[j];
      if (TreapHeight(pj) < TreapHeight(plast)) {
        plast->set_right(pj);
        s.push(plast);
      } else if (TreapHeight(pj) >= TreapHeight(proot)) {
        for (plast->update_subtree_data(); !s.empty(); s.pop())
          s.top()->update_subtree_data();
        pj->set_left(proot);
        proot = pj;
      } else {
        for (plast->update_subtree_data();
             TreapHeight(pj) >= TreapHeight(s.top()); s.pop()) {
          plast = s.top();
          plast->update_subtree_data();
        }
        pj->set_left(plast);
        s.top()->set_right(pj);
      }
      plast = pj;
    }
    for (plast->update_subtree_data(); !s.empty(); s.pop())
      s.top()->update_subtree_data();
    return proot;
  }

 protected:
  static TNode* JoinI(TNode* l, TNode* r) {
    if (TreapHeight(l) > TreapHeight(r)) {
      l->apply_deferred();
      l->set_right(l->right ? JoinI(l->right, r) : r);
      l->update_subtree_data();
      return l;
    } else {
      r->apply_deferred();
      r->set_left(r->left ? JoinI(l, r->left) : l);
      r->update_subtree_data();
      return r;
    }
  }

 public:
  static TNode* join(TNode* l, TNode* r) {
    return !l ? r : !r ? l : JoinI(l, r);
  }

  static TNode* join3(TNode* l, TNode* m1, TNode* r) {
    return join(l, join(m1, r));
  }

 protected:
  static void SplitByKeyI(TNode* p, const TKey& key, TNode*& output_l,
                          TNode*& output_r) {
    p->apply_deferred();
    if (p->key < key) {
      if (p->right) {
        output_l = p;
        SplitByKeyI(p->right, key, p->right, output_r);
        p->update_subtree_data();
        if (p->right) p->right->set_parent(p);
      } else {
        output_l = p;
        output_r = nullptr;
      }
    } else {
      if (p->left) {
        output_r = p;
        SplitByKeyI(p->left, key, output_l, p->left);
        p->update_subtree_data();
        if (p->left) p->left->set_parent(p);
      } else {
        output_l = nullptr;
        output_r = p;
      }
    }
  }

 public:
  static void split(TNode* root, const TKey& key, TNode*& output_l,
                    TNode*& output_r) {
    static_assert(use_key, "use_key should be true");
    if (!root) {
      output_l = output_r = nullptr;
    } else {
      SplitByKeyI(root, key, output_l, output_r);
      if (output_l) output_l->set_parent(nullptr);
      if (output_r) output_r->set_parent(nullptr);
    }
  }

 protected:
  static void SplitBySizeI(TNode* p, size_t lsize, TNode*& output_l,
                           TNode*& output_r) {
    p->apply_deferred();
    const size_t hlsize = bst::subtree_data::size(p->left);
    if (lsize < hlsize) {
      output_r = p;
      SplitBySizeI(p->left, lsize, output_l, p->left);
      if (p->left) p->left->set_parent(p);
    } else if (lsize == hlsize) {
      output_l = p->left;
      output_r = p;
      p->left = nullptr;
    } else if (lsize == hlsize + 1) {
      output_l = p;
      output_r = p->right;
      p->right = nullptr;
    } else {
      output_l = p;
      SplitBySizeI(p->right, lsize - hlsize - 1, p->right, output_r);
      if (p->right) p->right->set_parent(p);
    }
    p->update_subtree_data();
  }

 public:
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
      SplitBySizeI(root, lsize, output_l, output_r);
      if (output_l) output_l->set_parent(nullptr);
      if (output_r) output_r->set_parent(nullptr);
    }
  }

  static TNode* insert(TNode* root, TNode* node) {
    static_assert(use_key, "use_key should be true");
    if (!root) return node;
    root->apply_deferred();
    if (TreapHeight(root) >= TreapHeight(node)) {
      if (root->key < node->key) {
        root->set_right(insert(root->right, node));
      } else {
        root->set_left(insert(root->left, node));
      }
    } else {
      SplitByKeyI(root, node->key, node->left, node->right);
      if (node->left) node->left->set_parent(node);
      if (node->right) node->right->set_parent(node);
      root = node;
    }
    root->update_subtree_data();
    return root;
  }

  static TNode* remove(TNode* root, const TKey& key, TNode*& removed_node) {
    static_assert(use_key, "use_key should be true");
    if (!root) return root;
    root->apply_deferred();
    if (root->key < key) {
      root->set_right(remove(root->right, key, removed_node));
    } else if (root->key > key) {
      root->set_left(remove(root->left, key, removed_node));
    } else {
      removed_node = root;
      TNode* l = root->left;
      if (l) l->set_parent(nullptr);
      TNode* r = root->right;
      if (r) r->set_parent(nullptr);
      root->reset_links_and_update_subtree_data();
      return join(l, r);
    }
    root->update_subtree_data();
    return root;
  }

  static TNode* Union(TNode* p1, TNode* p2) {
    if (!p1) return p2;
    if (!p2) return p1;
    if (TreapHeight(p1) < TreapHeight(p2)) std::swap(p1, p2);
    TNode *pt1 = nullptr, *pt2 = nullptr;
    split(p2, p1->key, pt1, pt2);
    p1->set_left(Union(p1->left, pt1));
    p1->set_right(Union(p1->right, pt2));
    p1->update_subtree_data();
    return p1;
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
    subtree_data::propagate_to_root(p);
    return base::root(p);
  }
};
}  // namespace bst
