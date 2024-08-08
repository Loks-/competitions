#pragma once

namespace geometry {
namespace kdtree {
namespace action {
class None {
 public:
  static constexpr bool modify_data = false;
  static constexpr bool modify_tree = false;

  using TSelf = None;

 public:
  constexpr bool IsEmpty() const { return true; }

  constexpr void Clear() {};

  template <class TNode, class TActionValue>
  constexpr void Add(TNode*, const TActionValue&) {}

  template <class TNode>
  constexpr void Apply(const TNode*) {}
};
}  // namespace action
}  // namespace kdtree
}  // namespace geometry
