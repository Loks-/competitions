#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_root_to_node.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/base/sibling.h"
#include "common/binary_search_tree/base/swap.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/wavl_rank.h"
#include "common/binary_search_tree/node.h"
#include "common/binary_search_tree/tree.h"
#include "common/nodes_manager_fixed_size.h"

#include <algorithm>

namespace bst {
template <bool _use_parent, class TTData, class TTInfo = info::Size,
          class TTAction = action::None, class TTKey = int64_t,
          template <class> class TTNodesManager = NodesManagerFixedSize>
class WAVLTree
    : public Tree<TTNodesManager<Node<TTData, info::WAVLRank<TTInfo>, TTAction,
                                      true, _use_parent, false, TTKey>>,
                  WAVLTree<_use_parent, TTData, TTInfo, TTAction, TTKey,
                           TTNodesManager>> {
 public:
  static const bool use_key = true;
  static const bool use_parent = _use_parent;
  static const bool use_height = false;

  using TData = TTData;
  using TInfo = info::WAVLRank<TTInfo>;
  using TAction = TTAction;
  using TKey = TTKey;
  using TNode =
      Node<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
  using TSelf =
      WAVLTree<use_parent, TData, TTInfo, TAction, TKey, TTNodesManager>;
  using TTree = Tree<TTNodesManager<TNode>, TSelf>;
  friend class Tree<TTNodesManager<TNode>, TSelf>;

 public:
  explicit WAVLTree(size_t max_nodes) : TTree(max_nodes) {}

  static int Rank(TNode* node) { return node ? int(node->info.rank) : -1; }

  static int RankDiff(TNode* p, TNode* c) { return p ? Rank(p) - Rank(c) : 1; }

 protected:
  static void BuildTreeUpdateRankR(TNode* root) {
    if (root) {
      BuildTreeUpdateRankR(root->l);
      BuildTreeUpdateRankR(root->r);
      root->info.rank = std::max(Rank(root->l), Rank(root->r)) + 1;
    }
  }

 public:
  static TNode* BuildTree(const std::vector<TNode*>& nodes) {
    TNode* root = TTree::BuildTree(nodes);
    BuildTreeUpdateRankR(root);
    return root;
  }

 protected:
  static TNode* InsertByKeyIR(TNode* root, TNode* proot, TNode* node) {
    if (!root) return node;
    root->ApplyAction();
    TNode* new_root = root;
    if (root->key < node->key) {
      root->SetR(InsertByKeyIR(root->r, root, node));
      if (RankDiff(root, root->r) == 0) {
        if (RankDiff(root, root->l) == 1) {
          ++root->info.rank;
        } else if (RankDiff(root->r, root->r->r) == 1) {
          assert(RankDiff(root->r, root->r->l) == 2);
          new_root = root->r;
          --root->info.rank;
          Rotate<TNode, false, false>(new_root, root, proot);
        } else {
          assert(RankDiff(root->r, root->r->r) == 2);
          assert(RankDiff(root->r, root->r->l) == 1);
          new_root = root->r->l;
          --root->info.rank;
          --root->r->info.rank;
          ++new_root->info.rank;
          Rotate<TNode, false, false>(new_root, root->r, root);
          Rotate<TNode, false, false>(new_root, root, proot);
        }
      }
    } else {
      root->SetL(InsertByKeyIR(root->l, root, node));
      if (RankDiff(root, root->l) == 0) {
        if (RankDiff(root, root->r) == 1) {
          ++root->info.rank;
        } else if (RankDiff(root->l, root->l->l) == 1) {
          assert(RankDiff(root->l, root->l->r) == 2);
          new_root = root->l;
          --root->info.rank;
          Rotate<TNode, false, false>(new_root, root, proot);
        } else {
          assert(RankDiff(root->l, root->l->l) == 2);
          assert(RankDiff(root->l, root->l->r) == 1);
          new_root = root->l->r;
          --root->info.rank;
          --root->l->info.rank;
          ++new_root->info.rank;
          Rotate<TNode, false, false>(new_root, root->l, root);
          Rotate<TNode, false, false>(new_root, root, proot);
        }
      }
    }
    new_root->UpdateInfo();
    return new_root;
  }

  static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeFalse) {
    return InsertByKeyIR(root, nullptr, node);
  }

  static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeTrue) {
    return InsertByKeyIR(root, nullptr, node);
  }

 public:
  static TNode* InsertByKey(TNode* root, TNode* node) {
    assert(node);
    node->info.rank = 0;
    return root ? InsertByKeyI(root, node, TFakeBool<use_parent>()) : node;
  }

 protected:
  //   static TNode* SwapAndRemove(TNode* root, TNode* node) {
  //     root->ApplyAction();
  //     if (root->r) {
  //       root->SetR(SwapAndRemove(root->r, node));
  //       return FixBalance<true>(root);
  //     } else {
  //       TNode* child = root->l;
  //       root->SetL(node->l);
  //       root->SetR(node->r);
  //       node->l = root;  // Save information about swapped node
  //       return child;
  //     }
  //   }

 public:
  //   static TNode* RemoveByKey(TNode* root, const TKey& key,
  //                             TNode*& removed_node) {
  //     if (!root) return root;
  //     root->ApplyAction();
  //     if (root->key < key) {
  //       root->SetR(RemoveByKey(root->r, key, removed_node));
  //     } else if (root->key > key) {
  //       root->SetL(RemoveByKey(root->l, key, removed_node));
  //     } else {
  //       removed_node = root;
  //       if (root->l && root->r) {
  //         TNode* child = root->l;
  //         child->ApplyAction();
  //         if (child->r) {
  //           TNode* temp = SwapAndRemove(child, root);
  //           child = root->l;
  //           root->ResetLinksAndUpdateInfo();
  //           child->SetL(temp);
  //           return FixBalance<true>(child);
  //         } else {
  //           child->SetR(root->r);
  //           root->ResetLinksAndUpdateInfo();
  //           return FixBalance<true>(child);
  //         }
  //       } else {
  //         TNode* child = root->l ? root->l : root->r;
  //         root->ResetLinksAndUpdateInfo();
  //         return child;
  //       }
  //     }
  //     return FixBalance<true>(root);
  //   }

  static TNode* RemoveByNode(TNode* node) {
    static_assert(use_parent, "use_parent should be true");
    assert(node);
    action::ApplyRootToNode(node);

    // Optional swap
    if (node->l && node->r) {
      TNode* temp = node->l;
      for (temp->ApplyAction(); temp->r;) {
        temp = temp->r;
        temp->ApplyAction();
      }
      SwapAuto(node, node->p, temp, temp->p);
      std::swap(node->info.rank, temp->info.rank);
    }

    // Drop node from tree
    TNode* child = node->l ? node->l : node->r;
    TNode* parent = node->p;
    if (parent) {
      if (parent->l == node)
        parent->l = child;
      else
        parent->r = child;
    }
    if (child) child->p = parent;
    node->ResetLinksAndUpdateInfo();
    node->info.rank = 0;
    info::UpdateNodeToRoot(parent);

    if (parent && !parent->l && !parent->r && (Rank(parent) == 1)) {
      parent->info.rank = 0;
      child = parent;
      parent = parent->p;
    }

    for (; RankDiff(parent, child) > 2;) {
      assert(RankDiff(parent, child) == 3);
      TNode* sibling = Sibling(child, parent);
      if (RankDiff(parent, sibling) == 2) {
        --parent->info.rank;
      } else {
        assert((RankDiff(parent, sibling) == 1));
        if ((RankDiff(sibling, sibling->l) == 2) &&
            (RankDiff(sibling, sibling->r) == 2)) {
          --sibling->info.rank;
          --parent->info.rank;
        } else if (child == parent->l) {
          if (RankDiff(sibling, sibling->r) == 1) {
            --parent->info.rank;
            ++sibling->info.rank;
            RotateUp<TNode, true, true>(sibling);
            if (!parent->l && !parent->r) {
              parent->info.rank = 0;
            }
            break;
          } else {
            assert(sibling->l);
            parent->info.rank -= 2;
            --sibling->info.rank;
            sibling->l->info.rank += 2;
            RotateUp<TNode, false, true>(sibling->l);
            RotateUp<TNode, true, false>(parent->r);
            break;
          }
        } else {
          assert(child == parent->r);
          if (RankDiff(sibling, sibling->l) == 1) {
            --parent->info.rank;
            ++sibling->info.rank;
            RotateUp<TNode, true, true>(sibling);
            if (!parent->l && !parent->r) {
              parent->info.rank = 0;
            }
            break;
          } else {
            assert(sibling->r);
            parent->info.rank -= 2;
            --sibling->info.rank;
            sibling->r->info.rank += 2;
            RotateUp<TNode, false, true>(sibling->r);
            RotateUp<TNode, true, false>(parent->l);
            break;
          }
        }
      }
      child = parent;
      parent = parent->p;
    }
    return parent ? Root(parent) : child;
  }
};
}  // namespace bst
