#pragma once

template <class T>
inline unsigned ULogK(T x, T k) {
  unsigned l = 0;
  for (; x; x /= k) ++l;
  return (l > 0) ? l - 1 : 0;
}

template <class T>
inline unsigned ULog10(T x) {
  return ULogK(x, T(10));
}
