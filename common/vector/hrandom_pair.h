#pragma once

#include "common/hash.h"

#include <utility>
#include <vector>

namespace nvector {
template <class TValue = size_t>
constexpr std::vector<std::pair<TValue, TValue>> HRandomPair(size_t size,
                                                             size_t seed) {
  size_t h1 = 0, h2 = seed;
  std::vector<std::pair<TValue, TValue>> v(size);
  for (size_t i = 0; i < size; ++i) {
    h1 = HashCombine(h2, i);
    h2 = HashCombine(h1, i);
    v[i] = std::make_pair(h1, h2);
  }
  return v;
}

template <class TValue = size_t>
constexpr std::vector<std::pair<TValue, TValue>> HRandomPair(
    size_t size, size_t seed, const TValue& max_value) {
  size_t h1 = 0, h2 = seed;
  std::vector<std::pair<TValue, TValue>> v(size);
  for (size_t i = 0; i < size; ++i) {
    h1 = HashCombine(h2, i);
    h2 = HashCombine(h1, i);
    v[i] = std::make_pair(h1 % max_value, h2 % max_value);
  }
  return v;
}
}  // namespace nvector
