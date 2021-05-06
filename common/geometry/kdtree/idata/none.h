#pragma once

namespace geometry {
namespace kdtree {
namespace idata {
class None {
 public:
  static const bool support_box = false;

 public:
  None();

  template <class TPoint>
  void SetBox(const TPoint& pb, const TPoint& pe) {}
};
}  // namespace idata
}  // namespace kdtree
}  // namespace geometry
