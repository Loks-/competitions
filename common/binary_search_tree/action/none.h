#pragma once

namespace bst {
namespace action {
class None {
 public:
  using TSelf = None;

  static constexpr bool is_none = true;
  static constexpr bool modify_data = false;
  static constexpr bool modify_keys = false;
  static constexpr bool modify_tree = false;

 public:
  constexpr bool IsEmpty() const { return true; }

  constexpr void Clear() {};

  // External functions should not assume that Add and Apply are constexpr.
  template <class TNode, class TActionValue>
  constexpr void Add(TNode*, const TActionValue&) {}

  template <class TNode>
  constexpr void Apply(TNode*) {}
};
}  // namespace action
}  // namespace bst
