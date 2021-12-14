#pragma once

#include <numeric>
#include <vector>

namespace nvector {
template <class T>
inline T Sum(const std::vector<T>& v) {
  return std::accumulate(v.begin(), v.end(), T());
}
}  // namespace nvector
