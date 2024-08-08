#pragma once

#include "common/geometry/d2/angle.h"
#include "common/geometry/d2/long.h"
#include "common/geometry/d2/vector.h"

namespace geometry {
namespace d2 {
// Use faster comparison vs zero
class LAngle : public Angle<LongSigned, LAngle> {
 public:
  using T = LongSigned;
  using TSelf = LAngle;
  using TBase = Angle<LongSigned, TSelf>;

 public:
  constexpr LAngle() {}

  constexpr LAngle(const T& _dx, const T& _dy) : TBase(_dx, _dy) {
    Normalize();
  }

  constexpr explicit LAngle(const Vector<T>& v) : TBase(v) { Normalize(); }

  constexpr void Normalize() {
    assert(TBase::Valid());
    const T g = GCDSafe(TBase::dx, TBase::dy);
    TBase::dx /= g;
    TBase::dy /= g;
  }

 public:
  // Compare angle with pi keeping angle in the range [0, 2pi).
  constexpr bool CompareVSPi() const {
    return TBase::dy.Positive() || (TBase::dy.Empty() && TBase::dx.Positive());
  }

  // Compare angle with 0 keeping angle in the range [-pi, pi).
  constexpr bool CompareVS0() const {
    return TBase::dy.Negative() || (TBase::dy.Empty() && TBase::dx.Negative());
  }

  // Compare angles keeping them in the range [-pi, pi)
  constexpr bool ComparePiPi(const TSelf& r) const {
    return (CompareVS0() != r.CompareVS0()) ? CompareVS0() : TBase::CompareI(r);
  }

  // Compare angles keeping them in the range [0, 2pi)
  constexpr bool Compare02Pi(const TSelf& r) const {
    return (CompareVSPi() != r.CompareVSPi()) ? CompareVSPi()
                                              : TBase::CompareI(r);
  }

  constexpr bool operator<(const TSelf& r) const { return ComparePiPi(r); }
  constexpr bool operator>(const TSelf& r) const { return r < *this; }
  constexpr bool operator<=(const TSelf& r) const { return !(r < *this); }
  constexpr bool operator>=(const TSelf& r) const { return !(*this < r); }
};
}  // namespace d2
}  // namespace geometry

using L2Angle = geometry::d2::LAngle;
