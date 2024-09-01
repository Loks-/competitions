#pragma once

#include "common/binary_search_tree/base/find_by_key_greater.h"
#include "common/binary_search_tree/base/find_by_key_less.h"
#include "common/binary_search_tree/base/left.h"
#include "common/binary_search_tree/base/right.h"
#include "common/binary_search_tree/red_black_tree.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/memory/nodes_manager.h"

namespace ds {
namespace fus {
// Memory      -- O(S)
// Init        -- O(1)
// Insert      -- O(log S)
// HasKey      -- O(log S)
// Delete      -- O(log S)
// Size        -- O(1)
// Min         -- O(log S)
// Max         -- O(log S)
// Successor   -- O(log S)
// Predecessor -- O(log S)
class BinarySearchTree {
 protected:
  using TTree = bst::RedBlackTree<TEmpty, bst::info::None, bst::action::None,
                                  size_t, memory::NodesManager>;
  using TNode = TTree::TNode;

 protected:
  TTree tree;
  TNode* root;
  size_t usize;

 public:
  BinarySearchTree() : tree(0), root(nullptr), usize(0) {}

  BinarySearchTree(size_t u) : tree(u), root(nullptr), usize(u) {}

  void Clear() {
    tree.ReleaseTree(root);
    root = nullptr;
  }

  void Init(size_t u) {
    Clear();
    usize = u;
  }

  void Insert(size_t x) {
    if (HasKey(x)) return;
    root = tree.InsertNewNode(root, TEmpty(), x);
  }

  bool HasKey(size_t x) const { return tree.FindByKey(root, x); }

  void Delete(size_t x) {
    auto node = tree.FindByKey(root, x);
    if (node) root = tree.RemoveAndReleaseByNode(node);
  }

  size_t Size() const { return tree.Used(); }

  size_t USize() const { return usize; }

  size_t Min() const { return root ? bst::base::Left(root)->key : Empty; }

  size_t Max() const { return root ? bst::base::Right(root)->key : Empty; }

  size_t Successor(size_t x) const {
    auto node = bst::base::FindByKeyGreater(root, x + 1);
    return node ? node->key : Empty;
  }

  size_t Predecessor(size_t x) const {
    auto node = x ? bst::base::FindByKeyLess(root, x - 1) : nullptr;
    return node ? node->key : Empty;
  }
};
}  // namespace fus
}  // namespace ds
