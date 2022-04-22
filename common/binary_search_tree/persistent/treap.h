#pragma once
#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/treap_height.h"
#include "common/binary_search_tree/info/update_node_to_root.h"
#include "common/binary_search_tree/persistent/tree.h"
#include "common/memory/nodes_manager.h"

#include <stack>
#include <vector>

namespace bst {
namespace persistent {
template <bool use_key, bool use_parent, class TData,
          class TTInfo = bst::info::Size, class TAction = bst::action::None,
          class TKey = int64_t>
class Treap
    : public bst::persistent::Tree<
          memory::NodesManager<
              bst::base::Node<TData, bst::info::TreapHeight<unsigned, TTInfo>,
                              TAction, use_key, use_parent, TKey>>,
          Treap<use_key, use_parent, TData, TTInfo, TAction, TKey>> {
 public:
  static const bool support_remove = true;
  static const bool support_join = true;
  static const bool support_split = true;

  using TInfo = bst::info::TreapHeight<unsigned, TTInfo>;
  using THeight = typename TInfo::THeight;
  using TNode =
      bst::base::Node<TData, TInfo, TAction, use_key, use_parent, TKey>;
  using TSelf = Treap<use_key, use_parent, TData, TTInfo, TAction, TKey>;
  using TBase = bst::persistent::Tree<memory::NodesManager<TNode>, TSelf>;
  using TTree = bst::base::Tree<memory::NodesManager<TNode>, TSelf>;
  friend TTree;

 protected:
  static const THeight& Height(TNode* node) { return node->info.treap_height; }

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
      if (Height(pj) < Height(plast)) {
        plast->SetR(pj);
        s.push(plast);
      } else if (Height(pj) >= Height(proot)) {
        for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
        pj->SetL(proot);
        proot = pj;
      } else {
        for (plast->UpdateInfo(); Height(pj) >= Height(s.top()); s.pop()) {
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
    if (Height(root) >= Height(node)) {
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
