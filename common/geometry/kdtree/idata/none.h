#pragma once

namespace geometry {
namespace kdtree {
namespace idata {
class None {
 public:
  static constexpr bool support_box = false;

 public:
  template <class TPoint>
  constexpr void SetBox(const TPoint&, const TPoint&) {}
};
}  // namespace idata
}  // namespace kdtree
}  // namespace geometry
