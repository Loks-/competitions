#pragma once

#include "common/binary_search_tree/base/floor.h"
#include "common/binary_search_tree/base/left.h"
#include "common/binary_search_tree/base/lower_bound.h"
#include "common/binary_search_tree/base/right.h"
#include "common/binary_search_tree/red_black_tree_old.h"
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
  using TTree = bst::RedBlackTreeOld<MetaEmpty, std::tuple<>, std::tuple<>,
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
    tree.release_tree(root);
    root = nullptr;
  }

  void Init(size_t u) {
    Clear();
    usize = u;
  }

  void Insert(size_t x) {
    if (HasKey(x)) return;
    root = tree.insert_new(root, MetaEmpty(), x);
  }

  bool HasKey(size_t x) const { return tree.find(root, x); }

  void Delete(size_t x) {
    auto node = tree.find(root, x);
    if (node) root = tree.remove_and_release_node(node);
  }

  size_t Size() const { return tree.used(); }

  size_t USize() const { return usize; }

  size_t Min() const { return root ? bst::base::left(root)->key : kEmpty; }

  size_t Max() const { return root ? bst::base::right(root)->key : kEmpty; }

  size_t Successor(size_t x) const {
    auto node = bst::base::lower_bound(root, x + 1);
    return node ? node->key : kEmpty;
  }

  size_t Predecessor(size_t x) const {
    auto node = x ? bst::base::floor(root, x - 1) : nullptr;
    return node ? node->key : kEmpty;
  }
};
}  // namespace fus
}  // namespace ds
