#pragma once

#include <numeric>
#include <vector>

namespace nvector {
template <class T>
constexpr T Sum(const std::vector<T>& v) {
  return std::accumulate(v.begin(), v.end(), T());
}

template <class T>
constexpr T SumVV(const std::vector<std::vector<T>>& v) {
  T s = 0;
  for (auto& iv : v) s += Sum(iv);
  return s;
}

template <class T>
constexpr T SumVVV(const std::vector<std::vector<std::vector<T>>>& v) {
  T s = 0;
  for (auto& iv : v) s += SumVV(iv);
  return s;
}
}  // namespace nvector
