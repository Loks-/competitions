#pragma once

namespace geometry {
namespace kdtree {
namespace info {
class None {
 public:
  using TSelf = None;
  static const bool is_none = true;
  static const bool use_data = false;

  void Merge(const TSelf&) {}

  template <class TNode>
  void Update(const TNode*) {}

  template <class TNode, class TPoint>
  void UpdateLeaf(const TNode*, const TPoint&, const TPoint&) {}
};
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
