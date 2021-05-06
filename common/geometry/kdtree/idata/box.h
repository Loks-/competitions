#pragma once

#include "common/geometry/kdtree/idata/none.h"

namespace geometry {
namespace kdtree {
namespace idata {
template <class TTPoint>
class Box : public None {
 public:
  static const bool support_box = true;

  using TPoint = TTPoint;
  using TSelf = Box<TPoint>;
  using TBase = None;

 public:
  TPoint b, e;

 public:
  Box();

  void SetBox(const TPoint& pb, const TPoint& pe) {
    b = pb;
    e = pe;
  }
};
}  // namespace idata
}  // namespace kdtree
}  // namespace geometry
