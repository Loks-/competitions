#pragma once

#include "common/geometry/kdtree/base/volume.h"
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
  using TValue = typename TPoint::TType;

 public:
  TPoint b, e;

 public:
  void SetBox(const TPoint& pb, const TPoint& pe) {
    TBase::SetBox(pb, pe);
    b = pb;
    e = pe;
  }

  TValue Volume() const { return base::Volume(b, e); }
};
}  // namespace idata
}  // namespace kdtree
}  // namespace geometry
