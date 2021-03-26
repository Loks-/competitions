#pragma once

template <typename T>
inline int Sign(const T& val) {
  return (T() < val) - (val < T());
}
