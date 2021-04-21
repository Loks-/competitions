#pragma once

#include "common/geometry/d2/angle.h"
#include "common/geometry/d2/vector.h"
#include "common/numeric/utils/gcd.h"

namespace geometry {
namespace d2 {
template <class T>
class IAngle : public Angle<T, IAngle<T>> {
 public:
  using TSelf = IAngle<T>;
  using TBase = Angle<T, TSelf>;

  IAngle() {}
  IAngle(const T& _dx, const T& _dy) : TBase(_dx, _dy) { Normalize(); }
  explicit IAngle(const Vector<T>& v) : TBase(v) { Normalize(); }

  void Normalize() {
    assert(TBase::Valid());
    T g = GCDSafe<T>(TBase::dx, TBase::dy);
    TBase::dx /= g;
    TBase::dy /= g;
  }

 protected:
  bool CompareI_i128(const TSelf& r) const {
    return __int128_t(TBase::dy) * __int128_t(r.dx) <
           __int128_t(TBase::dx) * __int128_t(r.dy);
  }

 public:
  bool ComparePiPi_i128(const TSelf& r) const {
    return (TBase::CompareVS0() != r.CompareVS0()) ? TBase::CompareVS0()
                                                   : CompareI_i128(r);
  }

  bool Compare02Pi_i128(const TSelf& r) const {
    return (TBase::CompareVSPi() != r.CompareVSPi()) ? TBase::CompareVSPi()
                                                     : CompareI_i128(r);
  }
};
}  // namespace d2
}  // namespace geometry

using I2Angle = geometry::d2::IAngle<int64_t>;
