#pragma once

#include "common/base.h"

namespace graph {
namespace lcte {
namespace action {
class None {
 public:
  using TSelf = None;

  bool reverse = false;

  bool IsEmpty() const { return !reverse; }
  void Clear() { reverse = false; };

  template <class TNode>
  void Reverse(TNode*) {
    reverse = !reverse;
  }

  template <class TNode>
  void PApply(TNode* node) {
    if (reverse) {
      std::swap(node->l, node->r);
      if (node->l) node->l->Reverse();
      if (node->r) node->r->Reverse();
      reverse = false;
    }
  }

  template <class TNode>
  void VApply(TNode*) {
    assert(!reverse);
  }
};
}  // namespace action
}  // namespace lcte
}  // namespace graph
