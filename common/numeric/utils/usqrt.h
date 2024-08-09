#pragma once

#include <cmath>
#include <limits>

namespace numeric {
namespace hidden {
template <class T>
consteval T CTUSqrtHelper(T x, T l, T h) {
  if (l == h) return l;
  const T m = (l + h + 1) / 2;
  if (x / m < m)
    return CTUSqrtHelper<T>(x, l, m - 1);
  else
    return CTUSqrtHelper<T>(x, m, h);
}
}  // namespace hidden
}  // namespace numeric

template <class T>
consteval T CTUSqrt(T x) {
  return numeric::hidden::CTUSqrtHelper<T>(x, 0, x / 4 + 1);
}

template <class T>
constexpr T USqrt(T x) {
  T r = T(sqrt(double(x)));
  constexpr T sqrt_max = CTUSqrt<T>(std::numeric_limits<T>::max());
  for (; (r < sqrt_max) && (r * r < x);) ++r;
  for (; (r > sqrt_max) || (r * r > x);) --r;
  return r;
}
