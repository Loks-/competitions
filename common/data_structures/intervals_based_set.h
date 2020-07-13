#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/first_node.h"
#include "common/binary_search_tree/base/traversal.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/treap.h"
#include "common/nodes_manager.h"

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

    Interval() : b(0), e(0) {}
    explicit Interval(TValue v) : b(v), e(v + 1) {}
    Interval(TValue b_, TValue e_) : b(b_), e(e_) {}

    bool Empty() const { return e <= b; }
    TValue Size() const { return e - b; }
  };

  class Info : public bst::info::Size {
   public:
    using TBase = bst::info::Size;
    using TSelf = Info;
    static const bool use_data = true;

    TTValue set_size = 0;

    template <class TNode>
    void Update(TNode* node) {
      TBase::Update(node);
      set_size = node->data.Size() + (node->l ? node->l->info.set_size : 0) +
                 (node->r ? node->r->info.set_size : 0);
    }

    template <class TNode>
    void Insert(const TNode* node) {
      TBase::Insert(node);
      set_size += node->info.set_size;
    }

    template <class TNode>
    void Remove(const TNode* node) {
      TBase::Remove(node);
      set_size -= node->info.set_size;
    }
  };

  using TTree = bst::Treap<true, true, Interval, Info, bst::action::None,
                           TValue, NodesManager>;
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
  TValue Size() const { return Empty() ? 0 : root->info.set_size; }
  size_t TreeSize() const { return Empty() ? 0 : root->info.size; }

  std::vector<Interval> ToVector() const {
    return bst::Traverse<TNode, Interval>(root, bst::ETraversalOrder::Inorder);
  }

  bool HasKey(TValue key) const {
    for (TNode* node = root; node;) {
      if (key < node->data.b)
        node = node->l;
      else if (key >= node->data.e)
        node = node->r;
      else
        return true;
    }
    return false;
  }

  void Insert(TValue key) { Insert(Interval(key)); }

  void Insert(const Interval& i) {
    if (i.Empty()) return;
    auto b = i.b, e = i.e;
    TNode *l, *t, *m, *r;
    tree.SplitByKey(root, b, l, t);
    tree.SplitByKey(t, e, m, r);
    if (m) {
      b = std::min(b, bst::FirstNode(m)->data.b);
      tree.ReleaseTree(m);
    }
    if (r) {
      auto rf = bst::FirstNode(r);
      if (rf->data.b <= e) {
        e = rf->data.e;
        r = tree.RemoveAndReleaseByNode(rf);
      }
    }
    m = tree.New(Interval(b, e), e);
    root = tree.Join(l, tree.Join(m, r));
  }

 protected:
  void InsertIBSI(const TNode* node) {
    if (!node) return;
    Insert(node->data);
    InsertIBSI(node->l);
    InsertIBSI(node->r);
  }

 public:
  void Insert(const TSelf& ibs) { InsertIBSI(ibs.root); }

 protected:
  void ShiftValuesI(TNode* node, TValue shift) {
    if (!node) return;
    node->data.b += shift;
    node->data.e += shift;
    node->key += shift;
    ShiftValuesI(node->l, shift);
    ShiftValuesI(node->r, shift);
  }

 public:
  void ShiftValues(TValue shift) { ShiftValuesI(root, shift); }

  void Intersect(const Interval& i) {
    if (i.Empty()) {
      tree.ReleaseTree(root);
      root = nullptr;
      return;
    }
    TNode *l, *t, *m, *r;
    tree.SplitByKey(root, i.b + 1, l, t);
    tree.SplitByKey(t, i.e + 1, m, r);
    tree.ReleaseTree(l);
    if (m) {
      auto mf = bst::FirstNode(m);
      mf->data.b = std::max(mf->data.b, i.b);
    }
    if (r) {
      auto rf = bst::FirstNode(r);
      if (rf->data.b < i.e) {
        t = tree.New(Interval(rf->data.b, i.e), i.e);
        m = tree.Join(m, t);
      }
      tree.ReleaseTree(r);
    }
    root = m;
  }
};
}  // namespace ds
