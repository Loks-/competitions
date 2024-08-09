#pragma once

#include <cmath>
#include <limits>

namespace numeric {
namespace hidden {
template <class T>
consteval T CTUCbrtHelper(T x, T l, T h) {
  if (l == h) return l;
  const T m = (l + h + 1) / 2;
  if (x / m / m < m)
    return CTUCbrtHelper<T>(x, l, m - 1);
  else
    return CTUCbrtHelper<T>(x, m, h);
}
}  // namespace hidden
}  // namespace numeric

template <class T>
consteval T CTUCbrt(T x) {
  return numeric::hidden::CTUCbrtHelper<T>(x, 0, x / 8 + 1);
}

template <class T>
constexpr T UCbrt(T x) {
  T r = T(cbrt(double(x)));
  constexpr T cbrt_max = CTUCbrt<T>(std::numeric_limits<T>::max());
  for (; (r < cbrt_max) && (r * r * r < x);) ++r;
  for (; (r > cbrt_max) || (r * r * r > x);) --r;
  return r;
}
