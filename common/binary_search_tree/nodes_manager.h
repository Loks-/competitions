#pragma once

#include "common/base.h"
#include "common/template.h"
#include <random>
#include <stack>
#include <vector>

template <class TTNode>
class BSTNodesManager {
 public:
  using TNode = TTNode;
  using TData = typename TNode::TData;
  using TKey = typename TNode::TKey;

  static const bool use_key = TNode::use_key;
  static const bool use_height = TNode::use_height;

 protected:
  std::vector<TNode> nodes;
  unsigned used_nodes;
  std::stack<TNode*> released_nodes;
  std::minstd_rand random_engine;

 public:
  BSTNodesManager(unsigned max_nodes) : nodes(max_nodes), used_nodes(0) {}

 protected:
  void InitRandomHeightI(TNode* p, TFakeFalse) {}
  void InitRandomHeightI(TNode* p, TFakeTrue) { p->height = random_engine(); }

 public:
  void InitRandomHeight(TNode* p) {
    InitRandomHeightI(p, TFakeBool<use_height>());
  }

  TNode* GetNewNode() {
    if (!released_nodes.empty()) {
      TNode* p = released_nodes.top();
      p->ResetLinks();
      p->ClearAction();
      released_nodes.pop();
      return p;
    } else {
      assert(used_nodes < nodes.size());
      TNode* p = &(nodes[used_nodes++]);
      InitRandomHeight(p);
      return p;
    }
  }

  TNode* GetNewNode(const TData& data) {
    TNode* p = GetNewNode();
    p->data = data;
    p->UpdateInfo();
    return p;
  }

  TNode* GetNewNode(const TData& data, const TKey& key) {
    static_assert(use_key, "use_key should be true");
    TNode* p = GetNewNode();
    p->data = data;
    p->key = key;
    p->UpdateInfo();
    return p;
  }

  void ReleaseNode(TNode* p) { released_nodes.push(p); }

  unsigned UsedNodes() const {
    return used_nodes - unsigned(released_nodes.size());
  }

  unsigned AvailableNodes() const {
    return unsigned(nodes.size()) - UsedNodes();
  }

  TNode* GetNodeByRawIndex(unsigned index) { return &(nodes[index]); }
  void ResetNodes() {
    std::stack<TNode*>().swap(released_nodes);
    used_nodes = 0;
    for (TNode& node : nodes) {
      node.ResetLinks();
      node.ClearAction();
    }
  }
};
