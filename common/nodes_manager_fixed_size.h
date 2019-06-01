#pragma once

#include "common/base.h"
#include <stack>
#include <vector>

template <class TTNode>
class NodesManagerFixedSize {
 public:
  using TNode = TTNode;

 protected:
  std::vector<TNode> nodes;
  unsigned used_nodes;
  std::stack<TNode*> released_nodes;
  TNode* first;

 public:
  NodesManagerFixedSize(unsigned max_nodes)
      : nodes(max_nodes), used_nodes(0), first(&nodes[0]) {}

  void Reserve(unsigned new_max_nodes) {
    assert(new_max_nodes <= nodes.size());
  }

  void ReserveAdditional(unsigned new_nodes) {
    assert(new_nodes <= Reserved());
  }

  TNode* New() {
    if (!released_nodes.empty()) {
      TNode* p = released_nodes.top();
      p->ClearReuse();
      released_nodes.pop();
      return p;
    } else {
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

  TNode* NodeByRawIndex(unsigned index) { return first + index; }
  const TNode* NodeByRawIndex(unsigned index) const { return first + index; }
  unsigned RawIndex(const TNode* node) const { return node - first; }

  void ResetNodes() {
    std::stack<TNode*>().swap(released_nodes);
    used_nodes = 0;
    for (TNode& node : nodes) {
      node.ClearRelease();
      node.ClearReuse();
    }
  }
};
