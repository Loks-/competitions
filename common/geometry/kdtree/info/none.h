#pragma once

namespace geometry {
namespace kdtree {
namespace info {
class None {
 public:
  static constexpr bool is_none = true;
  static constexpr bool use_data = false;

  using TSelf = None;

 public:
  constexpr void Merge(const TSelf&) {}

  template <class TNode>
  constexpr void Update(const TNode*) {}

  template <class TNode, class TPoint>
  constexpr void UpdateLeaf(const TNode*, const TPoint&, const TPoint&) {}
};
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
