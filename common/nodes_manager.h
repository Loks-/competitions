#pragma once

#include <deque>
#include <stack>

template <class TTNode>
class NodesManager {
 public:
  using TNode = TTNode;

 protected:
  std::deque<TNode> nodes;
  unsigned used_nodes;
  std::stack<TNode*> released_nodes;

 public:
  NodesManager(unsigned reserve_nodes) : nodes(reserve_nodes), used_nodes(0) {}

  void Reserve(unsigned new_max_nodes) {
    if (new_max_nodes > nodes.size()) {
      nodes.resize(std::max(new_max_nodes, unsigned(2 * nodes.size())));
    }
  }

  void ReserveAdditional(unsigned new_nodes) {
    if (Reserved() < new_nodes) {
      Reserve(Used() + new_nodes);
    }
  }

  TNode* New() {
    if (!released_nodes.empty()) {
      TNode* p = released_nodes.top();
      p->ClearReuse();
      released_nodes.pop();
      return p;
    } else {
      ReserveAdditional(1);
      assert(used_nodes < nodes.size());
      TNode* p = &(nodes[used_nodes++]);
      p->ClearCreate();
      return p;
    }
  }

  void Release(TNode* p) {
    p->ClearRelease();
    released_nodes.push(p);
  }

  unsigned Size() const { return unsigned(nodes.size()); }

  unsigned Used() const { return used_nodes - unsigned(released_nodes.size()); }

  unsigned Reserved() const { return Size() - Used(); }

  TNode* NodeByRawIndex(unsigned index) { return &(nodes[index]); }
  const TNode* NodeByRawIndex(unsigned index) const { return &(nodes[index]); }

  void ResetNodes() {
    std::stack<TNode*>().swap(released_nodes);
    used_nodes = 0;
    for (TNode& node : nodes) {
      node.ClearRelease();
      node.ClearReuse();
    }
  }
};
