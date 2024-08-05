#pragma once

#include <numeric>
#include <vector>

namespace nvector {
template <class T>
constexpr T Product(const std::vector<T>& v) {
  T r(1);
  for (auto& x : v) r *= x;
  return r;
}
}  // namespace nvector
