#pragma once

template <class T>
inline T GCD(T x, T y) {
  for (; y;) {
    T z = x % y;
    x = y;
    y = z;
  }
  return x;
}
