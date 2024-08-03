#pragma once

template <class T>
inline int Sign(const T& val) {
  return (T() < val) - (val < T());
}
