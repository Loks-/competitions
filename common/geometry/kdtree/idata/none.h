#pragma once

namespace geometry {
namespace kdtree {
namespace idata {
class None {
 public:
  static const bool support_box = false;

 public:
  template <class TPoint>
  void SetBox(const TPoint&, const TPoint&) {}
};
}  // namespace idata
}  // namespace kdtree
}  // namespace geometry
