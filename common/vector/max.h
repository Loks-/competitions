#pragma once

#include <vector>

namespace nvector {
template <class T>
constexpr T Max(const std::vector<T>& v) {
  T r(v[0]);
  for (unsigned i = 1; i < v.size(); ++i)
    if (r < v[i]) r = v[i];
  return r;
}
}  // namespace nvector
