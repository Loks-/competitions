#pragma once

#include <vector>

namespace nvector {
template <class T>
inline T Min(const std::vector<T>& v) {
  T r(v[0]);
  for (unsigned i = 1; i < v.size(); ++i)
    if (v[i] < r) r = v[i];
  return r;
}
}  // namespace nvector
