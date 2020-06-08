#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_root_to_node.h"
#include "common/binary_search_tree/action/reverse.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/graph/tree.h"

namespace graph {
template <class TTData, class TTInfo, class TTAction = bst::action::Reverse>
class LinkCutTree {
 public:
  class LCTInfo : public TTInfo {
   public:
    using TBase = TTInfo;
    using TSelf = LCTInfo;

    void* lct_pp = nullptr;

    void* GetPP() { return lct_pp; }
    const void* GetPP() const { return lct_pp; }
    void SetPP(void* p) { lct_pp = p; }

    template <class TNode>
    void Update(TNode* node) {
      TBase::Update(node);
      if (node->l && node->l->info.lct_pp) {
        lct_pp = node->l->info.lct_pp;
        node->l->info.lct_pp = nullptr;
      }
      if (node->r && node->r->info.lct_pp) {
        lct_pp = node->r->info.lct_pp;
        node->r->info.lct_pp = nullptr;
      }
    }
  };

  using TData = TTData;
  using TInfo = LCTInfo;
  using TAction = TTAction;
  using TSelf = LinkCutTree<TData, TTInfo, TAction>;
  using TSTree = bst::SplayTree<false, TData, TInfo, TAction>;
  using TNode = typename TSTree::TNode;

 protected:
  void DisconnectR(TNode* node) {
    if (node->r) {
      node->r->SetP(nullptr);
      node->r->info.SetPP(node);
      node->r = nullptr;
      node->UpdateInfo();
    }
  }

 public:
  void Access(TNode* node) {
    assert(node);
    bst::action::ApplyRootToNode(node);
    TSTree::Splay(node);
    DisconnectR(node);
    for (TNode* v; (v = reinterpret_cast<TNode*>(node->info.GetPP()));) {
      bst::action::ApplyRootToNode(v);
      TSTree::Splay(v);
      DisconnectR(v);
      v->r = node;
      node->info.lct_pp = nullptr;
      bst::Rotate<TNode, true, false>(node, v, nullptr);
    }
  }

  TNode* FindRoot(TNode* node) {
    Access(node);
    while (node->l) {
      node = node->l;
      node->ApplyAction();
    }
    TSTree::Splay(node);
    return node;
  }

  void Cut(TNode* node) {
    Access(node);
    if (node->l) {
      node->l->SetP(nullptr);
      node->l = nullptr;
      node->UpdateInfo();
    }
  }

  // Link root to node
  void Link(TNode* root, TNode* node) {
    Access(root);
    assert(root->l == nullptr);
    Access(node);
    node->SetP(root);
    root->l = node;
    root->UpdateInfo();
  }

  // Set new root
  void SetRoot(TNode* node) {
    assert(node);
    Access(node);
    node->action.ReverseSubtree(node);
  }

 protected:
  TSTree stree;

 protected:
  void BuildR(const TreeGraph& tree, unsigned node, unsigned p) {
    for (unsigned c : tree.Edges(node)) {
      if (c == p) continue;
      Node(c)->info.SetPP(Node(node));
      BuildR(tree, c, node);
    }
  }

 public:
  LinkCutTree() {}
  explicit LinkCutTree(const TreeGraph& tree) { Build(tree); }
  LinkCutTree(const TreeGraph& tree, const std::vector<TData>& data) {
    Build(tree, data);
  }

  TNode* Node(unsigned x) { return stree.NodeByRawIndex(x); }
  const TNode* Node(unsigned x) const { return stree.NodeByRawIndex(x); }

  void Build(const TreeGraph& tree) {
    stree.Reset(tree.Size());
    for (unsigned i = 0; i < tree.Size(); ++i) stree.New();
    BuildR(tree, tree.GetRoot(), CNone);
  }

  void Build(const TreeGraph& tree, const std::vector<TData>& data) {
    assert(tree.Size() == data.size());
    stree.Reset(tree.Size());
    for (unsigned i = 0; i < tree.Size(); ++i) stree.New(data[i]);
    BuildR(tree, tree.GetRoot(), CNone);
  }

  void SetData(unsigned x, const TData& data) {
    auto node = Node(x);
    Access(node);
    node->data = data;
    node->UpdateInfo();
  }

  // Reset tree root
  const TInfo& GetPathInfo(unsigned x, unsigned y) {
    SetRoot(Node(x));
    Access(Node(y));
    return Node(y)->info;
  }

  // Reset tree root
  template <class TActionValue>
  void AddActionOnPath(unsigned x, unsigned y, const TActionValue& value) {
    SetRoot(Node(x));
    Access(Node(y));
    Node(y)->AddAction(value);
  }
};
}  // namespace graph
