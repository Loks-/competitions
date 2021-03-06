#pragma once

#include "common/hash.h"

#include <vector>

namespace nvector {
template <class TValue = size_t>
inline std::vector<TValue> HRandom(size_t size, size_t seed) {
  size_t h = seed;
  std::vector<TValue> v(size);
  for (size_t i = 0; i < size; ++i) {
    h = HashCombine(h, i);
    v[i] = h;
  }
  return v;
}

template <class TValue = size_t>
inline std::vector<TValue> HRandom(size_t size, size_t seed,
                                   const TValue& max_value) {
  size_t h = seed;
  std::vector<TValue> v(size);
  for (size_t i = 0; i < size; ++i) {
    h = HashCombine(h, i);
    v[i] = h % max_value;
  }
  return v;
}
}  // namespace nvector
