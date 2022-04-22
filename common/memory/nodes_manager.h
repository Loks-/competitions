#pragma once

#include "common/base.h"

#include <algorithm>
#include <deque>
#include <stack>

namespace memory {
// TNode should be based on memory::Node class.
template <class TTNode>
class NodesManager {
 public:
  using TNode = TTNode;

 protected:
  std::deque<TNode> nodes;
  size_t used_nodes;
  std::stack<TNode*> released_nodes;

 public:
  explicit NodesManager(size_t reserve_nodes)
      : nodes(reserve_nodes), used_nodes(0) {}
  NodesManager() : NodesManager(0) {}

  void Reserve(size_t new_max_nodes) {
    if (new_max_nodes > nodes.size()) {
      nodes.resize(std::max(new_max_nodes, 2 * nodes.size()));
    }
  }

  void ReserveAdditional(size_t new_nodes) {
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

  size_t Size() const { return nodes.size(); }
  size_t Used() const { return used_nodes - released_nodes.size(); }
  size_t Reserved() const { return Size() - Used(); }

  TNode* NodeByRawIndex(size_t index) { return &(nodes[index]); }
  const TNode* NodeByRawIndex(size_t index) const { return &(nodes[index]); }

  void ResetNodes() {
    std::stack<TNode*>().swap(released_nodes);
    used_nodes = 0;
    for (TNode& node : nodes) {
      node.ClearRelease();
      node.ClearReuse();
    }
  }
};
}  // namespace memory
