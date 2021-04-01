#pragma once

#include "common/binary_search_tree/action/none.h"

#include <algorithm>

namespace bst {
namespace action {
class Reverse : public None {
 public:
  using TBase = None;
  using TSelf = Reverse;
  static const bool modify_tree = true;

  bool reverse;

  Reverse() : reverse(false) {}
  bool IsEmpty() const { return !reverse; }
  void Clear() { reverse = false; };

  template <class TNode>
  void ReverseSubtree(TNode*) {
    reverse = !reverse;
  }

  template <class TNode>
  void Add(TNode* node, bool value) {
    if (value) ReverseSubtree(node);
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (reverse) {
      std::swap(node->l, node->r);
      if (node->l) node->l->action.ReverseSubtree(node->l);
      if (node->r) node->r->action.ReverseSubtree(node->r);
      reverse = false;
    }
  }
};
}  // namespace action
}  // namespace bst
