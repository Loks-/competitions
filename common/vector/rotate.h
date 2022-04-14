#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace nvector {
template <class TIterator>
inline void SwapIntervals(const TIterator& b, const TIterator& m,
                          const TIterator& e) {
  std::reverse(b, m);
  std::reverse(m, e);
  std::reverse(b, e);
}

template <class T>
inline void RotateLeft(std::vector<T>& v, size_t shift) {
  size_t size = v.size();
  if (size > 0) SwapIntervals(v.begin(), v.begin() + (shift % size), v.end());
}

template <class T>
inline void RotateRight(std::vector<T>& v, size_t shift) {
  RotateLeft(v, v.size() - (shift % v.size()));
}
}  // namespace nvector
