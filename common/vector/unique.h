#pragma once

#include <algorithm>
#include <vector>

namespace nvector {
template <class T>
inline void Unique(std::vector<T>& v) {
  v.erase(std::unique(v.begin(), v.end()), v.end());
}

template <class T>
inline void UniqueUnsorted(std::vector<T>& v) {
  std::sort(v.begin(), v.end());
  Unique(v);
}
}  // namespace nvector
