#pragma once

template <typename T>
inline T Abs(const T& val) {
  return (val < T(0)) ? -val : val;
}
