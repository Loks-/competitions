#pragma once

#include <algorithm>
#include <vector>

namespace nvector {
// Union with duplicates
template <class T>
inline std::vector<T> MUnionV(const std::vector<T>& v1,
                              const std::vector<T>& v2) {
  if (v1.empty()) return v2;
  if (v2.empty()) return v1;
  std::vector<T> r;
  r.reserve(v1.size() + v2.size());
  unsigned i1 = 0, i2 = 0;
  for (; (i1 < v1.size()) && (i2 < v2.size());) {
    if (v1[i1] < v2[i2]) {
      r.push_back(v1[i1++]);
    } else {
      r.push_back(v2[i2++]);
    }
  }
  r.insert(r.end(), v1.begin() + i1, v1.end());
  r.insert(r.end(), v2.begin() + i2, v2.end());
  return r;
}

template <class T>
inline std::vector<T> MUnionVUnsorted(std::vector<T>& v1, std::vector<T>& v2) {
  auto v = v1;
  v.insert(v.end(), v2.begin(), v2.end());
  std::sort(v.begin(), v.end());
  return v;
}
}  // namespace nvector
