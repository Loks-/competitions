#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/wavl_rank.h"
#include "common/nodes_manager_fixed_size.h"

namespace bst {
template <bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t>
class WAVLTree
    : public base::BalancedTree<
          NodesManagerFixedSize<base::Node<TData, info::WAVLRank<TInfo>,
                                           TAction, true, use_parent, TKey>>,
          WAVLTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  using TNode =
      base::Node<TData, info::WAVLRank<TInfo>, TAction, true, use_parent, TKey>;
  using TSelf = WAVLTree<use_parent, TData, TInfo, TAction, TKey>;
  using TBTree = base::BalancedTree<NodesManagerFixedSize<TNode>, TSelf>;
  using TTree = typename TBTree::TTree;
  friend TBTree;
  friend TTree;

 public:
  explicit WAVLTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static int Rank(TNode* node) { return node ? int(node->info.rank) : -1; }

  static int RankDiff(TNode* p, TNode* c) { return p ? Rank(p) - Rank(c) : 1; }

  static TNode* BuildTreeI(const std::vector<TNode*>& vnodes, size_t first,
                           size_t last) {
    if (first >= last) return nullptr;
    size_t m = (first + last) / 2;
    TNode* root = vnodes[m];
    root->SetL(BuildTreeI(vnodes, first, m));
    root->SetR(BuildTreeI(vnodes, m + 1, last));
    root->UpdateInfo();
    root->info.rank = std::max(Rank(root->l), Rank(root->r)) + 1;
    return root;
  }

  static TNode* FixBalanceInsertRotate1(TNode* child, TNode* parent) {
    --parent->info.rank;
    base::Rotate<TNode, true, false>(child, parent, nullptr);
    return child;
  }

  static TNode* FixBalanceInsertRotate2(TNode* gchild, TNode* child,
                                        TNode* parent) {
    assert(RankDiff(child, gchild) == 1);
    assert(RankDiff(child, Sibling(gchild, child)) == 2);
    --parent->info.rank;
    --child->info.rank;
    ++gchild->info.rank;
    base::Rotate<TNode, false, false>(gchild, child, parent);
    base::Rotate<TNode, true, false>(gchild, parent, nullptr);
    return gchild;
  }

  static TNode* FixBalanceInsert(TNode* root) {
    if (RankDiff(root, root->r) == 0) {
      if (RankDiff(root, root->l) == 1) {
        ++root->info.rank;
      } else if (RankDiff(root->r, root->r->r) == 1) {
        assert(RankDiff(root->r, root->r->l) == 2);
        return FixBalanceInsertRotate1(root->r, root);
      } else {
        return FixBalanceInsertRotate2(root->r->l, root->r, root);
      }
    } else if (RankDiff(root, root->l) == 0) {
      if (RankDiff(root, root->r) == 1) {
        ++root->info.rank;
      } else if (RankDiff(root->l, root->l->l) == 1) {
        assert(RankDiff(root->l, root->l->r) == 2);
        return FixBalanceInsertRotate1(root->l, root);
      } else {
        return FixBalanceInsertRotate2(root->l->r, root->l, root);
      }
    }
    return root;
  }

  static TNode* FixBalanceRemoveRotate1(TNode* child, TNode* parent) {
    --parent->info.rank;
    ++child->info.rank;
    base::Rotate<TNode, true, true>(child, parent, nullptr);
    if (!parent->l && !parent->r) {
      parent->info.rank = 0;
    }
    return child;
  }

  static TNode* FixBalanceRemoveRotate2(TNode* gchild, TNode* child,
                                        TNode* parent) {
    assert(gchild);
    parent->info.rank -= 2;
    child->info.rank -= 1;
    gchild->info.rank += 2;
    base::Rotate<TNode, false, true>(gchild, child, parent);
    base::Rotate<TNode, true, false>(gchild, parent, nullptr);
    return gchild;
  }

  static TNode* FixBalanceRemove(TNode* root) {
    if (!root->l && !root->r) {
      root->info.rank = 0;
      return root;
    }
    TNode* child = (RankDiff(root, root->l) > 2)
                       ? root->l
                       : (RankDiff(root, root->r) > 2) ? root->r : root;
    if (child == root) return root;
    assert(RankDiff(root, child) == 3);
    TNode* sibling = base::Sibling(child, root);
    if (RankDiff(root, sibling) == 2) {
      --root->info.rank;
      return root;
    }
    assert((RankDiff(root, sibling) == 1));
    if ((RankDiff(sibling, sibling->l) == 2) &&
        (RankDiff(sibling, sibling->r) == 2)) {
      --sibling->info.rank;
      --root->info.rank;
      return root;
    } else if (child == root->l) {
      if (RankDiff(sibling, sibling->r) == 1) {
        return FixBalanceRemoveRotate1(sibling, root);
      } else {
        return FixBalanceRemoveRotate2(sibling->l, sibling, root);
      }
    } else {
      if (RankDiff(sibling, sibling->l) == 1) {
        return FixBalanceRemoveRotate1(sibling, root);
      } else {
        return FixBalanceRemoveRotate2(sibling->r, sibling, root);
      }
    }
  }
};
}  // namespace bst
