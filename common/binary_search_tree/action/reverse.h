#pragma once

#include "common/binary_search_tree/action/none.h"

#include <algorithm>

namespace bst {
namespace action {
class Reverse : public None {
 public:
  using TBase = None;
  using TSelf = Reverse;
  static const bool is_none = false;

  bool reverse;

  Reverse() : reverse(false) {}
  bool IsEmpty() const { return !reverse; }
  void Clear() { reverse = false; };

  void ReverseSubtree() { reverse = !reverse; }

  template <class TNode>
  void Add(TNode*, bool value) {
    if (value) ReverseSubtree();
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (reverse) {
      std::swap(node->l, node->r);
      if (node->l) node->l->action.ReverseSubtree();
      if (node->r) node->r->action.ReverseSubtree();
      reverse = false;
    }
  }
};
}  // namespace action
}  // namespace bst
