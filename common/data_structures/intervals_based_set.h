#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/left.h"
#include "common/binary_search_tree/base/to_vector.h"
#include "common/binary_search_tree/subtree_data/base.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/treap.h"
#include "common/memory/nodes_manager.h"

#include <algorithm>
#include <vector>

namespace ds {
template <class TTValue = int64_t>
class IntervalsBasedSet {
 public:
  using TValue = TTValue;
  using TSelf = IntervalsBasedSet<TValue>;

  class Interval {
   public:
    TValue b, e;

   public:
    constexpr Interval() : b(0), e(0) {}
    constexpr explicit Interval(TValue v) : b(v), e(v + 1) {}
    constexpr Interval(TValue b_, TValue e_) : b(b_), e(e_) {}
    constexpr bool Empty() const { return e <= b; }
    constexpr TValue Size() const { return e - b; }
  };

  class BSTSubtreeData : public bst::subtree_data::Base {
   public:
    using Self = BSTSubtreeData;

    static constexpr bool use_data = true;

   public:
    TValue set_size = 0;

   public:
    template <class TNode>
    constexpr void update(TNode* node) {
      Base::update(node);
      set_size = node->data.Size() + get(node->left) + get(node->right);
    }

    template <class TNode>
    constexpr TValue get(const TNode* node) const {
      return node ? node->subtree_data.template get<Self>().set_size : 0;
    }
  };

  using TTree = bst::Treap<true, true, Interval,
                           std::tuple<bst::subtree_data::Size, BSTSubtreeData>,
                           std::tuple<>, TValue, memory::NodesManager>;
  using TNode = typename TTree::TNode;

 protected:
  TTree my_tree;
  TTree& tree;
  TNode* root;

 public:
  IntervalsBasedSet() : my_tree(0), tree(my_tree), root(nullptr) {}

  explicit IntervalsBasedSet(TTree& tree_to_use)
      : my_tree(0), tree(tree_to_use), root(nullptr) {}

  bool Empty() const { return root == nullptr; }

  TValue Size() const {
    return Empty() ? 0
                   : root->subtree_data.template get<BSTSubtreeData>().set_size;
  }

  size_t TreeSize() const { return bst::subtree_data::size(root); }

  std::vector<Interval> ToVector() const { return bst::base::to_vector(root); }

  bool HasKey(TValue key) const {
    for (TNode* node = root; node;) {
      if (key < node->data.b) {
        node = node->left;
      } else if (key >= node->data.e) {
        node = node->right;
      } else {
        return true;
      }
    }
    return false;
  }

  void Insert(TValue key) { Insert(Interval(key)); }

  void Insert(const Interval& i) {
    if (i.Empty()) return;
    auto b = i.b, e = i.e;
    TNode *l, *t, *m, *r;
    tree.split(root, b, l, t);
    tree.split(t, e, m, r);
    if (m) {
      b = std::min(b, bst::base::left(m)->data.b);
      tree.release_tree(m);
    }
    if (r) {
      auto rf = bst::base::left(r);
      if (rf->data.b <= e) {
        e = rf->data.e;
        r = tree.remove_and_release_node(rf);
      }
    }
    m = tree.create_node(Interval(b, e), e);
    root = tree.join(l, tree.join(m, r));
  }

 protected:
  void InsertIBSI(const TNode* node) {
    if (!node) return;
    Insert(node->data);
    InsertIBSI(node->left);
    InsertIBSI(node->right);
  }

 public:
  void Insert(const TSelf& ibs) { InsertIBSI(ibs.root); }

 protected:
  void ShiftValuesI(TNode* node, TValue shift) {
    if (!node) return;
    node->data.b += shift;
    node->data.e += shift;
    node->key += shift;
    ShiftValuesI(node->left, shift);
    ShiftValuesI(node->right, shift);
  }

 public:
  void ShiftValues(TValue shift) { ShiftValuesI(root, shift); }

  void Intersect(const Interval& i) {
    if (i.Empty()) {
      tree.release_tree(root);
      root = nullptr;
      return;
    }
    TNode *l, *t, *m, *r;
    tree.split(root, i.b + 1, l, t);
    tree.split(t, i.e + 1, m, r);
    tree.release_tree(l);
    if (m) {
      const auto mf = bst::base::left(m);
      mf->data.b = std::max(mf->data.b, i.b);
    }
    if (r) {
      const auto rf = bst::base::left(r);
      if (rf->data.b < i.e) {
        t = tree.create_node(Interval(rf->data.b, i.e), i.e);
        m = tree.join(m, t);
      }
      tree.release_tree(r);
    }
    root = m;
  }
};
}  // namespace ds
