#pragma once

#include "common/base.h"

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
  constexpr void Reset(size_t max_nodes) {
    nodes.clear();
    nodes.resize(max_nodes);
    used_nodes = 0;
    first = &(nodes[0]);
  }

  constexpr explicit NodesManagerFixedSize(size_t max_nodes) {
    Reset(max_nodes);
  }

  constexpr NodesManagerFixedSize() : NodesManagerFixedSize(0) {}

  constexpr void Reserve([[maybe_unused]] size_t new_max_nodes) {
    assert(new_max_nodes <= nodes.size());
  }

  constexpr void ReserveAdditional([[maybe_unused]] size_t new_nodes) {
    assert(new_nodes <= Reserved());
  }

  constexpr TNode* New() {
    if (!released_nodes.empty()) {
      TNode* p = released_nodes.top();
      p->reuse();
      released_nodes.pop();
      return p;
    } else {
      assert(used_nodes < nodes.size());
      TNode* p = &(nodes[used_nodes]);
      p->initialize(used_nodes++);
      return p;
    }
  }

  constexpr void Release(TNode* p) {
    p->release();
    released_nodes.push(p);
  }

  constexpr size_t Size() const { return nodes.size(); }

  constexpr size_t Used() const { return used_nodes - released_nodes.size(); }

  constexpr size_t Reserved() const { return Size() - Used(); }

  constexpr TNode* NodeByRawIndex(size_t index) { return first + index; }

  constexpr const TNode* NodeByRawIndex(size_t index) const {
    return first + index;
  }

  constexpr size_t RawIndex(const TNode* node) const { return node - first; }

  constexpr void ResetNodes() {
    std::stack<TNode*>().swap(released_nodes);
    used_nodes = 0;
    for (TNode& node : nodes) {
      node.release();
      node.reuse();
    }
  }
};
}  // namespace memory
