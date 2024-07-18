#pragma once

#include "common/vector/unique.h"

#include <algorithm>
#include <vector>

namespace nvector {
template <class T>
inline unsigned Union(const std::vector<T>& v1, const std::vector<T>& v2) {
  unsigned r = 0, i1 = 0, i2 = 0;
  for (; (i1 < v1.size()) && (i2 < v2.size()); ++r) {
    if (v1[i1] < v2[i2]) {
      ++i1;
    } else if (v2[i2] < v1[i1]) {
      ++i2;
    } else {
      ++i1;
      ++i2;
    }
  }
  return r + (v1.size() - i1) + (v2.size() - i2);
}

template <class T>
inline std::vector<T> UnionV(const std::vector<T>& v1,
                             const std::vector<T>& v2) {
  std::vector<T> r;
  unsigned i1 = 0, i2 = 0;
  for (; (i1 < v1.size()) && (i2 < v2.size());) {
    if (v1[i1] < v2[i2]) {
      r.push_back(v1[i1++]);
    } else if (v2[i2] < v1[i1]) {
      r.push_back(v2[i2++]);
    } else {
      r.push_back(v1[i1++]);
      ++i2;
    }
  }
  r.insert(r.end(), v1.begin() + i1, v1.end());
  r.insert(r.end(), v2.begin() + i2, v2.end());
  return r;
}

template <class T>
inline unsigned UnionUnsorted(std::vector<T>& v1, std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return Union(v1, v2);
}

template <class T>
inline std::vector<T> UnionVUnsorted(std::vector<T>& v1, std::vector<T>& v2) {
  auto v = v1;
  v.insert(v.end(), v2.begin(), v2.end());
  return UniqueUnsorted(v);
}
}  // namespace nvector
