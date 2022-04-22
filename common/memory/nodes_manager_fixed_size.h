#pragma once

#include "common/base.h"
#include "common/template.h"

#include <stack>
#include <vector>

namespace memory {
// TNode should be based on memory::Node class.
template <class TTNode>
class NodesManagerFixedSize {
 public:
  using TNode = TTNode;

 protected:
  std::vector<TNode> nodes;
  size_t used_nodes;
  std::stack<TNode*> released_nodes;
  TNode* first;

 public:
  void Reset(size_t max_nodes) {
    nodes.clear();
    nodes.resize(max_nodes);
    used_nodes = 0;
    first = &(nodes[0]);
  }

  explicit NodesManagerFixedSize(size_t max_nodes) { Reset(max_nodes); }
  NodesManagerFixedSize() : NodesManagerFixedSize(0) {}

  void Reserve(size_t new_max_nodes) {
    FakeUse(new_max_nodes);
    assert(new_max_nodes <= nodes.size());
  }

  void ReserveAdditional(size_t new_nodes) {
    FakeUse(new_nodes);
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

  size_t Size() const { return nodes.size(); }
  size_t Used() const { return used_nodes - released_nodes.size(); }
  size_t Reserved() const { return Size() - Used(); }

  TNode* NodeByRawIndex(size_t index) { return first + index; }
  const TNode* NodeByRawIndex(size_t index) const { return first + index; }
  size_t RawIndex(const TNode* node) const { return node - first; }

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
