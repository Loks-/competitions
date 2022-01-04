#pragma once

#include <vector>

namespace nvector {
template <class T>
inline auto Make(unsigned d1, const T& value) {
  return std::vector<T>(d1, value);
}

template <class T>
inline auto Make(unsigned d1, unsigned d2, const T& value) {
  return std::vector<std::vector<T>>(d1, Make<T>(d2, value));
}

template <class T>
inline auto Make(unsigned d1, unsigned d2, unsigned d3, const T& value) {
  return std::vector<std::vector<std::vector<T>>>(d1, Make<T>(d2, d3, value));
}
}  // namespace nvector
