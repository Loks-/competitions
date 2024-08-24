#pragma once

#include "common/binary_search_tree/action/none.h"

#include <algorithm>

namespace bst {
namespace action {
class Reverse : public None {
 public:
  using TBase = None;
  using TSelf = Reverse;

  static constexpr bool is_none = false;
  static constexpr bool modify_tree = true;

 public:
  bool reverse;

 public:
  constexpr Reverse() : reverse(false) {}

  constexpr bool IsEmpty() const { return !reverse; }

  constexpr void Clear() { reverse = false; };

  template <class TNode>
  constexpr void ReverseSubtree(TNode*) {
    reverse = !reverse;
  }

  template <class TNode>
  constexpr void Add(TNode* node, bool value) {
    if (value) ReverseSubtree(node);
  }

  template <class TNode>
  constexpr void Apply(TNode* node) {
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
