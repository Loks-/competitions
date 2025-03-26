#pragma once
#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/persistent/tree.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/treap_height.h"
#include "common/binary_search_tree/subtree_data/utils/propagate_to_root.h"
#include "common/memory/nodes_manager.h"

#include <stack>
#include <vector>

namespace bst {
namespace persistent {
template <bool use_key, bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t>
class Treap
    : public bst::persistent::Tree<
          memory::NodesManager<bst::base::Node<
              TData,
              base::SubtreeData<templates::PrependT<subtree_data::TreapHeight,
                                                    TAggregatorsTuple>>,
              base::Deferred<TDeferredTuple>, use_key, use_parent, TKey>>,
          Treap<use_key, use_parent, TData, TAggregatorsTuple, TDeferredTuple,
                TKey>> {
 public:
  static constexpr bool support_remove = true;
  static constexpr bool support_join = true;
  static constexpr bool support_split = true;

  using TTreapHeight = subtree_data::TreapHeight;
  using TSubtreeData = base::SubtreeData<
      templates::PrependT<subtree_data::TreapHeight, TAggregatorsTuple>>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode = bst::base::Node<TData, TSubtreeData, TDeferred, use_key,
                                use_parent, TKey>;
  using TSelf = Treap<use_key, use_parent, TData, TAggregatorsTuple,
                      TDeferredTuple, TKey>;
  using TBase = bst::persistent::Tree<memory::NodesManager<TNode>, TSelf>;
  using TTree = bst::base::Tree<memory::NodesManager<TNode>, TSelf>;
  friend TTree;

 protected:
  static constexpr unsigned TreapHeight(const TNode* node) {
    return bst::subtree_data::TreapHeight::get(node);
  }

 public:
  Treap() : TBase(0) {}
  explicit Treap(size_t expected_nodes) : TBase(expected_nodes) {}

 public:
  // Same code as for bst::Treap::BuildTree
  static TNode* BuildTree(const std::vector<TNode*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    TNode* proot = nodes[0];
    TNode* plast = proot;
    std::stack<TNode*> s;
    for (size_t j = 1; j < nodes.size(); ++j) {
      TNode* pj = nodes[j];
      if (TreapHeight(pj) < TreapHeight(plast)) {
        plast->SetR(pj);
        s.push(plast);
      } else if (TreapHeight(pj) >= TreapHeight(proot)) {
        for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
        pj->SetL(proot);
        proot = pj;
      } else {
        for (plast->UpdateInfo(); TreapHeight(pj) >= TreapHeight(s.top());
             s.pop()) {
          plast = s.top();
          plast->UpdateInfo();
        }
        pj->SetL(plast);
        s.top()->SetR(pj);
      }
      plast = pj;
    }
    for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
    return proot;
  }

 protected:
  void SplitByKeyI(TNode* p, const TKey& key, TNode*& output_l,
                   TNode*& output_r) {
    p->ApplyAction();
    if (p->key < key) {
      if (p->r) {
        p = TBase::PClone(p);
        output_l = p;
        SplitByKeyI(p->r, key, p->r, output_r);
        TBase::UpdatePForChildren(p);
        p->UpdateInfo();
      } else {
        output_l = p;
        output_r = nullptr;
      }
    } else {
      if (p->l) {
        p = TBase::PClone(p);
        output_r = p;
        SplitByKeyI(p->l, key, output_l, p->l);
        TBase::UpdatePForChildren(p);
        p->UpdateInfo();
      } else {
        output_l = nullptr;
        output_r = p;
      }
    }
  }

 public:
  void SplitByKey(TNode* root, const TKey& key, TNode*& output_l,
                  TNode*& output_r) {
    static_assert(use_key, "use_key should be true");
    if (!root) {
      output_l = output_r = nullptr;
    } else {
      SplitByKeyI(root, key, output_l, output_r);
      if (output_l) output_l->SetP(nullptr);
      if (output_r) output_r->SetP(nullptr);
    }
  }

  TNode* InsertByKey(TNode* root, TNode* node) {
    static_assert(use_key, "use_key should be true");
    if (!root) return node;
    root->ApplyAction();
    if (TreapHeight(root) >= TreapHeight(node)) {
      root = TBase::PClone(root);
      if (root->key < node->key)
        root->r = InsertByKey(root->r, node);
      else
        root->l = InsertByKey(root->l, node);
    } else {
      SplitByKeyI(root, node->key, node->l, node->r);
      root = node;
    }
    TBase::UpdatePForChildren(root);
    root->UpdateInfo();
    return root;
  }
};
}  // namespace persistent
}  // namespace bst
