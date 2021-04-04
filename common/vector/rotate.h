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
inline void RotateLeft(const std::vector<T>& v, size_t shift) {
  size_t size = v.size();
  if (size > 0) {
    SwapIntervals(v.begin(), v.begin() + (shift % size), v.end());
  }
}
}  // namespace nvector
