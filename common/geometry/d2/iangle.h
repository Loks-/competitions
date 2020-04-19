#pragma once

#include "common/base.h"
#include "common/geometry/d2/vector.h"
#include "common/numeric/utils/gcd.h"

#include <cmath>

namespace geometry {
namespace d2 {
// Angle is represented as complex point dx + i*dy.
template <class T>
class IAngle {
 public:
  using TSelf = IAngle<T>;

  T dx, dy;

  IAngle() : dx(1), dy(0) {}
  IAngle(const T& _dx, const T& _dy) : dx(_dx), dy(_dy) { Normalize(); }
  explicit IAngle(const Vector<T>& v) : dx(v.dx), dy(v.dy) { Normalize(); }

  bool Valid() const { return (dx != 0) || (dy != 0); }

  void Normalize() {
    assert(Valid());
    T g = GCD<T>(dx < 0 ? -dx : dx, dy < 0 ? -dy : dy);
    dx /= g;
    dy /= g;
  }

  double Angle() const {
    assert(Valid());
    return atan2(dy, dx);
  }

  TSelf operator-() const { return TSelf(dx, -dy); }

  TSelf operator+(const TSelf& r) const {
    return TSelf(dx * r.dx - dy * r.dy, dx * r.dy + dy * r.dx);
  }

  TSelf operator-(const TSelf& r) const {
    return TSelf(dx * r.dx + dy * r.dy, -dx * r.dy + dy * r.dx);
  }

 protected:
  bool CompareI(const TSelf& r) const { return dy * r.dx < dx * r.dy; }

  bool CompareI_i128(const TSelf& r) const {
    return __int128_t(dy) * __int128_t(r.dx) <
           __int128_t(dx) * __int128_t(r.dy);
  }

 public:
  // Compare angle with pi keeping angle in the range [0, 2pi).
  bool CompareVSPi() const { return (dy > 0) || ((dy == 0) && (dx > 0)); }

  // Compare angle with 0 keeping angle in the range [-pi, pi).
  bool CompareVS0() const { return (dy < 0) || ((dy == 0) && (dx < 0)); }

  // Compare angles keeping them in the range [-pi, pi)
  bool ComparePiPi(const TSelf& r) const {
    return (CompareVS0() != r.CompareVS0()) ? CompareVS0() : CompareI(r);
  }

  bool ComparePiPi_i128(const TSelf& r) const {
    return (CompareVS0() != r.CompareVS0()) ? CompareVS0() : CompareI_i128(r);
  }

  // Compare angles keeping them in the range [0, 2pi)
  bool Compare02Pi(const TSelf& r) const {
    return (CompareVSPi() != r.CompareVSPi()) ? CompareVSPi() : CompareI(r);
  }

  bool Compare02Pi_i128(const TSelf& r) const {
    return (CompareVSPi() != r.CompareVSPi()) ? CompareVSPi()
                                              : CompareI_i128(r);
  }

  bool operator==(const TSelf& r) const { return (dx == r.dx) && (dy == r.dy); }
  bool operator!=(const TSelf& r) const { return (dx != r.dx) || (dy != r.dy); }
  bool operator<(const TSelf& r) const { return ComparePiPi(r); }
  bool operator>(const TSelf& r) const { return r < *this; }
  bool operator<=(const TSelf& r) const { return !(r < *this); }
  bool operator>=(const TSelf& r) const { return !(*this < r); }
};
}  // namespace d2
}  // namespace geometry

using I2Angle = geometry::d2::IAngle<int64_t>;
