#pragma once

#include <algorithm>
#include <vector>

namespace nvector {
template <class T>
inline unsigned Intersection(const std::vector<T>& v1,
                             const std::vector<T>& v2) {
  unsigned r = 0;
  for (unsigned i1 = 0, i2 = 0; (i1 < v1.size()) && (i2 < v2.size());) {
    if (v1[i1] < v2[i2]) {
      ++i1;
    } else if (v2[i2] < v1[i1]) {
      ++i2;
    } else {
      ++r;
      ++i1;
      ++i2;
    }
  }
  return r;
}

template <class T>
inline std::vector<T> IntersectionV(const std::vector<T>& v1,
                                    const std::vector<T>& v2) {
  std::vector<T> r;
  for (unsigned i1 = 0, i2 = 0; (i1 < v1.size()) && (i2 < v2.size());) {
    if (v1[i1] < v2[i2]) {
      ++i1;
    } else if (v2[i2] < v1[i1]) {
      ++i2;
    } else {
      r.push_back(v1[i1]);
      ++i1;
      ++i2;
    }
  }
  return r;
}

template <class T>
inline unsigned IntersectionUnsorted(std::vector<T>& v1, std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return Intersection(v1, v2);
}

template <class T>
inline std::vector<T> IntersectionVUnsorted(std::vector<T>& v1,
                                            std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return Intersection(v1, v2);
}
}  // namespace nvector
