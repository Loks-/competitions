#pragma once

#include <cmath>
#include <limits>

template <class T>
consteval T CTUSqrt(T x, T l, T h) {
  if (l == h) return l;
  const T m = (l + h + 1) / 2;
  if (x / m < m)
    return CTUSqrt<T>(x, l, m - 1);
  else
    return CTUSqrt<T>(x, m, h);
}

template <class T>
consteval T CTUSqrt(T x) {
  return CTUSqrt<T>(x, 0, x / 4 + 1);
}

template <class T>
constexpr T USqrt(T x) {
  T r = T(sqrt(double(x)));
  constexpr T sqrt_max = CTUSqrt<T>(std::numeric_limits<T>::max());
  for (; (r < sqrt_max) && (r * r < x);) ++r;
  for (; (r > sqrt_max) || (r * r > x);) --r;
  return r;
}
