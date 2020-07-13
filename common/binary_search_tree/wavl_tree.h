#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/root.h"
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

  static TNode* InsertRotate1(TNode* child, TNode* parent) {
    --parent->info.rank;
    Rotate<TNode, false, false>(child, parent, nullptr);
    return child;
  }

  static TNode* InsertRotate2(TNode* gchild, TNode* child, TNode* parent) {
    assert(RankDiff(child, gchild) == 1);
    assert(RankDiff(child, Sibling(gchild, child)) == 2);
    --parent->info.rank;
    --child->info.rank;
    ++gchild->info.rank;
    Rotate<TNode, false, false>(gchild, child, parent);
    Rotate<TNode, false, false>(gchild, parent, nullptr);
    return gchild;
  }

  static TNode* InsertByKeyIR(TNode* root, TNode* node) {
    if (!root) return node;
    root->ApplyAction();
    TNode* new_root = root;
    if (root->key < node->key) {
      root->SetR(InsertByKeyIR(root->r, node));
      if (RankDiff(root, root->r) == 0) {
        if (RankDiff(root, root->l) == 1) {
          ++root->info.rank;
        } else if (RankDiff(root->r, root->r->r) == 1) {
          assert(RankDiff(root->r, root->r->l) == 2);
          new_root = InsertRotate1(root->r, root);
        } else {
          new_root = InsertRotate2(root->r->l, root->r, root);
        }
      }
    } else {
      root->SetL(InsertByKeyIR(root->l, node));
      if (RankDiff(root, root->l) == 0) {
        if (RankDiff(root, root->r) == 1) {
          ++root->info.rank;
        } else if (RankDiff(root->l, root->l->l) == 1) {
          assert(RankDiff(root->l, root->l->r) == 2);
          new_root = InsertRotate1(root->l, root);
        } else {
          new_root = InsertRotate2(root->l->r, root->l, root);
        }
      }
    }
    new_root->UpdateInfo();
    return new_root;
  }

  //   static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeFalse) {
  //     return InsertByKeyIR(root, node);
  //   }

  //   static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeTrue) {
  //     return InsertByKeyIR(root, node);
  //   }

 public:
  static TNode* InsertByKey(TNode* root, TNode* node) {
    assert(node);
    node->info.rank = 0;
    // return root ? InsertByKeyI(root, node, TFakeBool<use_parent>()) : node;
    return root ? InsertByKeyIR(root, node) : node;
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

 protected:
  static void RemoveRotate1(TNode* child, TNode* parent) {
    --parent->info.rank;
    ++child->info.rank;
    Rotate<TNode, true, true>(child, parent, parent->p);
    if (!parent->l && !parent->r) {
      parent->info.rank = 0;
    }
  }

  static void RemoveRotate2(TNode* gchild, TNode* child, TNode* parent) {
    assert(gchild);
    parent->info.rank -= 2;
    child->info.rank -= 1;
    gchild->info.rank += 2;
    Rotate<TNode, false, true>(gchild, child, parent);
    Rotate<TNode, true, false>(gchild, parent, parent->p);
  }

  //   static TNode* RemoveByKeyIR(TNode* root, const TKey& key,
  //                               TNode*& removed_node) {
  //     if (!root) return root;
  //     root->ApplyAction();
  //     if (root->key < key) {
  //       root->SetR(child = RemoveByKey(root->r, key, removed_node));
  //       TNode* child = root->r;
  //     } else if (root->key > key) {
  //       root->SetL(child = RemoveByKey(root->l, key, removed_node));
  //     } else {
  //       //   removed_node = root;
  //       //   if (root->l && root->r) {
  //       //     TNode* child = root->l;
  //       //     child->ApplyAction();
  //       //     if (child->r) {
  //       //       TNode* temp = SwapAndRemove(child, root);
  //       //       child = root->l;
  //       //       root->ResetLinksAndUpdateInfo();
  //       //       child->SetL(temp);
  //       //       return FixBalance<true>(child);
  //       //     } else {
  //       //       child->SetR(root->r);
  //       //       root->ResetLinksAndUpdateInfo();
  //       //       return FixBalance<true>(child);
  //       //     }
  //       //   } else {
  //       //     TNode* child = root->l ? root->l : root->r;
  //       //     root->ResetLinksAndUpdateInfo();
  //       //     return child;
  //       //   }
  //     }
  //     return FixBalance<true>(root);
  //   }

  static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                             TFakeFalse) {
    return TTree::RemoveByKeyIR(root, key);
  }

  static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                             TFakeTrue) {
    return TTree::RemoveByKeyI(root, key, removed_node, TFakeTrue{});
  }

  static TNode* RemoveByNodeI(TNode* node) {
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

    for (; RankDiff(parent, child) > 2; parent = (child = parent)->p) {
      assert(RankDiff(parent, child) == 3);
      TNode* sibling = Sibling(child, parent);
      if (RankDiff(parent, sibling) == 2) {
        --parent->info.rank;
        continue;
      }
      assert((RankDiff(parent, sibling) == 1));
      if ((RankDiff(sibling, sibling->l) == 2) &&
          (RankDiff(sibling, sibling->r) == 2)) {
        --sibling->info.rank;
        --parent->info.rank;
        continue;
      } else if (child == parent->l) {
        if (RankDiff(sibling, sibling->r) == 1) {
          RemoveRotate1(sibling, parent);
        } else {
          RemoveRotate2(sibling->l, sibling, parent);
        }
      } else {
        if (RankDiff(sibling, sibling->l) == 1) {
          RemoveRotate1(sibling, parent);
        } else {
          RemoveRotate2(sibling->r, sibling, parent);
        }
      }
      break;
    }
    return parent ? Root(parent) : child;
  }
};  // namespace bst
}  // namespace bst
