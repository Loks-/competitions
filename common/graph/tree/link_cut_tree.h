#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/rotate.h"
#include "common/binary_search_tree/deferred/reverse.h"
#include "common/binary_search_tree/deferred/utils/propagate_to_node.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/subtree_data/base.h"
#include "common/graph/tree.h"
#include "common/templates/tuple.h"

namespace graph {
template <class TTData, class TAggregatorsTuple,
          class TDeferredTuple = std::tuple<>>
class LinkCutTree {
 public:
  class LCTSubtreeData : public bst::subtree_data::Base {
   public:
    using Self = LCTSubtreeData;

    void* lct_pp = nullptr;

    void* GetPP() { return lct_pp; }
    const void* GetPP() const { return lct_pp; }
    void SetPP(void* p) { lct_pp = p; }

    template <class TNode>
    void update(TNode* node) {
      if (node->left && node->left->subtree_data.template get<Self>().lct_pp) {
        lct_pp = node->left->subtree_data.template get<Self>().lct_pp;
        node->left->subtree_data.template get<Self>().lct_pp = nullptr;
      }
      if (node->right &&
          node->right->subtree_data.template get<Self>().lct_pp) {
        lct_pp = node->right->subtree_data.template get<Self>().lct_pp;
        node->right->subtree_data.template get<Self>().lct_pp = nullptr;
      }
    }
  };

  using TData = TTData;
  using TAggregators = templates::AppendT<LCTSubtreeData, TAggregatorsTuple>;
  using TDeferreds =
      templates::PrependIfMissingT<bst::deferred::Reverse, TDeferredTuple>;
  using TSelf = LinkCutTree<TData, TAggregatorsTuple, TDeferredTuple>;
  using TSTree = bst::SplayTree<false, TData, TAggregators, TDeferreds>;
  using TInfo = typename TSTree::TInfo;
  using TNode = typename TSTree::TNode;

 protected:
  void DisconnectR(TNode* node) {
    if (node->right) {
      node->right->set_parent(nullptr);
      node->right->subtree_data.template get<LCTSubtreeData>().SetPP(node);
      node->right = nullptr;
      node->update_subtree_data();
    }
  }

 public:
  void Access(TNode* node) {
    assert(node);
    bst::deferred::propagate_to_node(node);
    TSTree::Splay(node);
    DisconnectR(node);
    for (TNode* v;
         (v = reinterpret_cast<TNode*>(
              node->subtree_data.template get<LCTSubtreeData>().GetPP()));) {
      bst::deferred::propagate_to_node(v);
      TSTree::Splay(v);
      DisconnectR(v);
      v->right = node;
      node->subtree_data.template get<LCTSubtreeData>().lct_pp = nullptr;
      bst::base::Rotate<TNode, true, false>(node, v, nullptr);
    }
  }

  TNode* FindRoot(TNode* node) {
    Access(node);
    while (node->left) {
      node = node->left;
      node->apply_deferred();
    }
    TSTree::Splay(node);
    return node;
  }

  void Cut(TNode* node) {
    Access(node);
    if (node->left) {
      node->left->set_parent(nullptr);
      node->left = nullptr;
      node->update_subtree_data();
    }
  }

  // Link root to node
  void Link(TNode* root, TNode* node) {
    Access(root);
    assert(root->left == nullptr);
    Access(node);
    node->set_parent(root);
    root->left = node;
    root->update_subtree_data();
  }

  // Set new root
  void SetRoot(TNode* node) {
    assert(node);
    Access(node);
    bst::deferred::reverse_subtree(node);
  }

 protected:
  TSTree stree;

 protected:
  void BuildR(const TreeGraph& tree, unsigned node, unsigned p) {
    for (unsigned c : tree.Edges(node)) {
      if (c == p) continue;
      Node(c)->subtree_data.template get<LCTSubtreeData>().SetPP(Node(node));
      BuildR(tree, c, node);
    }
  }

 public:
  LinkCutTree() {}
  explicit LinkCutTree(const TreeGraph& tree) { Build(tree); }
  explicit LinkCutTree(unsigned size) { Build(std::vector<TData>(size)); }
  explicit LinkCutTree(const std::vector<TData>& data) { Build(data); }
  LinkCutTree(const TreeGraph& tree, const std::vector<TData>& data) {
    Build(tree, data);
  }

  TNode* Node(unsigned x) { return stree.at(x); }
  const TNode* Node(unsigned x) const { return stree.at(x); }

  void Build(const TreeGraph& tree) {
    stree.reset(tree.Size());
    for (unsigned i = 0; i < tree.Size(); ++i) stree.New();
    BuildR(tree, tree.GetRoot(), CNone);
  }

  void Build(const TreeGraph& tree, const std::vector<TData>& data) {
    assert(tree.Size() == data.size());
    stree.reset(tree.Size());
    for (unsigned i = 0; i < tree.Size(); ++i) stree.New(data[i]);
    BuildR(tree, tree.GetRoot(), CNone);
  }

  void Build(const std::vector<TData>& data) {
    stree.reset(data.size());
    for (unsigned i = 0; i < data.size(); ++i) stree.New(data[i]);
  }

  void SetData(unsigned x, const TData& data) {
    auto node = Node(x);
    Access(node);
    node->data = data;
    node->update_subtree_data();
  }

  // Reset tree root
  const TInfo& GetPathInfo(unsigned x, unsigned y) {
    SetRoot(Node(x));
    Access(Node(y));
    return Node(y)->subtree_data;
  }

  // Reset tree root
  template <class TAction, class TActionValue>
  void AddActionOnPath(unsigned x, unsigned y, const TActionValue& value) {
    SetRoot(Node(x));
    Access(Node(y));
    TAction::add(Node(y), value);
  }
};
}  // namespace graph
