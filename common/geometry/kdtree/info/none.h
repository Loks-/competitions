#pragma once

namespace geometry {
namespace kdtree {
namespace info {
class None {
 public:
  using TSelf = None;
  static const bool is_none = true;
  static const bool use_data = false;

  void Merge(const TSelf& r);

  template <class TNode>
  void Update(const TNode*) {}
};
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
