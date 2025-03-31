#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/wavl_rank.h"
#include "common/memory/contiguous_nodes_manager.h"
#include "common/templates/tuple.h"

#include <tuple>

namespace bst {
template <bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t>
class WAVLTree
    : public base::BalancedTree<
          memory::ContiguousNodesManager<base::Node<
              TData,
              base::SubtreeData<templates::PrependT<subtree_data::WAVLRank,
                                                    TAggregatorsTuple>>,
              base::Deferred<TDeferredTuple>, use_parent, true, TKey>>,
          WAVLTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple,
                   TKey>> {
 public:
  using TSubtreeData = base::SubtreeData<
      templates::PrependT<subtree_data::WAVLRank, TAggregatorsTuple>>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode =
      base::Node<TData, TSubtreeData, TDeferred, use_parent, true, TKey>;
  using TSelf =
      WAVLTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple, TKey>;
  using TBTree =
      base::BalancedTree<memory::ContiguousNodesManager<TNode>, TSelf>;
  using TTree = typename TBTree::Base;
  friend TBTree;
  friend TTree;

  static constexpr bool support_join3 = true;

 public:
  explicit WAVLTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static constexpr int Rank(TNode* node) {
    return node ? subtree_data::WAVLRank::get(node) : -1;
  }

  static constexpr int RankDiff(TNode* p, TNode* c) {
    return p ? Rank(p) - Rank(c) : 1;
  }

  static constexpr void SetRank(TNode* node, int rank) {
    subtree_data::WAVLRank::set(node, rank);
  }

  static constexpr void UpdateRank(TNode* p) {
    SetRank(p, std::max(Rank(p->left), Rank(p->right)) + 1);
  }

  static constexpr void IncRank(TNode* node) {
    subtree_data::WAVLRank::inc(node);
  }

  static constexpr void DecRank(TNode* node) {
    subtree_data::WAVLRank::dec(node);
  }

  static TNode* build_tree_impl(const std::vector<TNode*>& vnodes, size_t first,
                                size_t last) {
    if (first >= last) return nullptr;
    size_t m = (first + last) / 2;
    TNode* root = vnodes[m];
    root->set_left(build_tree_impl(vnodes, first, m));
    root->set_right(build_tree_impl(vnodes, m + 1, last));
    root->update_subtree_data();
    UpdateRank(root);
    return root;
  }

  static TNode* FixBalanceInsertRotate1(TNode* child, TNode* parent) {
    DecRank(parent);
    base::Rotate<true, false, TNode>(child, parent, nullptr);
    return child;
  }

  static TNode* FixBalanceInsertRotate2(TNode* gchild, TNode* child,
                                        TNode* parent) {
    assert(RankDiff(child, gchild) == 1);
    assert(RankDiff(child, base::sibling(gchild, child)) == 2);
    DecRank(parent);
    DecRank(child);
    IncRank(gchild);
    base::Rotate<false, true, TNode>(gchild, child, parent);
    base::Rotate<true, false, TNode>(gchild, parent, nullptr);
    return gchild;
  }

  static TNode* fix_balance_insert(TNode* root) {
    if (RankDiff(root, root->right) == 0) {
      if (RankDiff(root, root->left) == 1) {
        IncRank(root);
      } else if (RankDiff(root->right, root->right->right) == 1) {
        assert(RankDiff(root->right, root->right->left) == 2);
        return FixBalanceInsertRotate1(root->right, root);
      } else {
        return FixBalanceInsertRotate2(root->right->left, root->right, root);
      }
    } else if (RankDiff(root, root->left) == 0) {
      if (RankDiff(root, root->right) == 1) {
        IncRank(root);
      } else if (RankDiff(root->left, root->left->left) == 1) {
        assert(RankDiff(root->left, root->left->right) == 2);
        return FixBalanceInsertRotate1(root->left, root);
      } else {
        return FixBalanceInsertRotate2(root->left->right, root->left, root);
      }
    }
    return root;
  }

  static TNode* FixBalanceRemoveRotate1(TNode* child, TNode* parent) {
    DecRank(parent);
    IncRank(child);
    base::Rotate<true, true, TNode>(child, parent, nullptr);
    if (!parent->left && !parent->right) {
      SetRank(parent, 0);
    }
    return child;
  }

  static TNode* FixBalanceRemoveRotate2(TNode* gchild, TNode* child,
                                        TNode* parent) {
    assert(gchild);
    SetRank(parent, Rank(parent) - 2);
    DecRank(child);
    SetRank(gchild, Rank(gchild) + 2);
    base::Rotate<false, true, TNode>(gchild, child, parent);
    base::Rotate<true, false, TNode>(gchild, parent, nullptr);
    return gchild;
  }

  static TNode* fix_balance_remove(TNode* root) {
    if (!root->left && !root->right) {
      SetRank(root, 0);
      return root;
    }
    TNode* child = (RankDiff(root, root->left) > 2)    ? root->left
                   : (RankDiff(root, root->right) > 2) ? root->right
                                                       : root;
    if (child == root) return root;
    assert(RankDiff(root, child) == 3);
    TNode* sibling = base::sibling(child, root);
    if (RankDiff(root, sibling) == 2) {
      DecRank(root);
      return root;
    }
    assert((RankDiff(root, sibling) == 1));
    if ((RankDiff(sibling, sibling->left) == 2) &&
        (RankDiff(sibling, sibling->right) == 2)) {
      DecRank(sibling);
      DecRank(root);
      return root;
    } else if (child == root->left) {
      if (RankDiff(sibling, sibling->right) == 1) {
        return FixBalanceRemoveRotate1(sibling, root);
      } else {
        return FixBalanceRemoveRotate2(sibling->left, sibling, root);
      }
    } else {
      if (RankDiff(sibling, sibling->left) == 1) {
        return FixBalanceRemoveRotate1(sibling, root);
      } else {
        return FixBalanceRemoveRotate2(sibling->right, sibling, root);
      }
    }
  }

  static TNode* join3_impl(TNode* l, TNode* m1, TNode* r) {
    TTree::join3_impl(l, m1, r);
    UpdateRank(m1);
    return m1;
  }

  static TNode* Join3L(TNode* l, TNode* m1, TNode* r, int hr) {
    if (Rank(l) > hr + 1) {
      l->apply_deferred();
      l->set_right(Join3L(l->right, m1, r, hr));
      l->update_subtree_data();
      assert(Rank(l) >= Rank(l->right));
      return fix_balance_insert(l);
    } else {
      return join3_impl(l, m1, r);
    }
  }

  static TNode* Join3R(TNode* l, TNode* m1, TNode* r, int hl) {
    if (Rank(r) > hl + 1) {
      r->apply_deferred();
      r->set_left(Join3R(l, m1, r->left, hl));
      r->update_subtree_data();
      assert(Rank(r) >= Rank(r->left));
      return fix_balance_insert(r);
    } else {
      return join3_impl(l, m1, r);
    }
  }

 public:
  static TNode* join3(TNode* l, TNode* m1, TNode* r) {
    assert(m1 && !m1->left && !m1->right);
    const auto hl = Rank(l), hr = Rank(r), hd = hl - hr;
    return (hd > 1)    ? Join3L(l, m1, r, hr)
           : (hd < -1) ? Join3R(l, m1, r, hl)
                       : join3_impl(l, m1, r);
  }
};
}  // namespace bst
