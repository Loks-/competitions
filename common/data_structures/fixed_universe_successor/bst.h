#pragma once

#include "common/binary_search_tree/base/find_by_key_greater.h"
#include "common/binary_search_tree/base/find_by_key_less.h"
#include "common/binary_search_tree/red_black_tree.h"
#include "common/nodes_manager.h"

namespace ds {
namespace fus {
// Memory      -- O(S)
// Init        -- O(1)
// Insert      -- O(log S)
// Has         -- O(log S)
// Delete      -- O(log S)
// Size        -- O(1)
// Successor   -- O(log S)
// Predecessor -- O(log S)
class BST {
 protected:
  using TTree = bst::RedBlackTree<TEmpty, bst::info::None, bst::action::None,
                                  size_t, NodesManager>;
  using TNode = TTree::TNode;
  TTree tree;
  TNode* root;
  size_t usize;
  const size_t missed = -size_t(1);

 public:
  BST() : tree(0), root(nullptr), usize(0) {}
  BST(size_t u) : tree(u), root(nullptr), usize(u) {}

  void Clear() {
    tree.ReleaseTree(root);
    root = nullptr;
  }

  void Init(size_t u) {
    Clear();
    usize = u;
  }

  void Insert(size_t x) {
    if (Has(x)) return;
    root = tree.InsertNewNode(root, TEmpty(), x);
  }

  bool Has(size_t x) const { return tree.FindByKey(root, x); }

  void Delete(size_t x) {
    auto node = tree.FindByKey(root, x);
    if (node) root = tree.RemoveAndReleaseByNode(node);
  }

  size_t Size() const { return tree.Used(); }
  size_t USize() const { return usize; }

  size_t Successor(size_t x) const {
    auto node = bst::base::FindByKeyGreater(root, x + 1);
    return node ? node->key : missed;
  }

  size_t Predecessor(size_t x) const {
    auto node = bst::base::FindByKeyLess(root, x - 1);
    return node ? node->key : missed;
  }
};
}  // namespace fus
}  // namespace ds
