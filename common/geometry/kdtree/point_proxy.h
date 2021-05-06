#pragma once

namespace geometry {
namespace kdtree {
template <class TTPoint>
class PointProxy {
 public:
  using TPoint = TTPoint;
  using TValue = typename TPoint::T;

  static TValue DGet(unsigned, const TPoint&);
  static void DSet(unsigned, TPoint&, const TValue&);
  static TPoint DSetCopy(unsigned, const TPoint&, const TValue&);
  static bool Under(const TPoint& l, const TPoint& r);
  static bool StrictUnder(const TPoint& l, const TPoint& r);
};
}  // namespace kdtree
}  // namespace geometry
