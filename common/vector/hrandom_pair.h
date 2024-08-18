#pragma once

#include "common/hash/combine.h"

#include <utility>
#include <vector>

namespace nvector {
template <class TValue = size_t>
constexpr std::vector<std::pair<TValue, TValue>> HRandomPair(size_t size,
                                                             size_t seed) {
  size_t h1 = 0, h2 = seed;
  if (h1 == h2) nhash::DCombineH(h2, h1);
  std::vector<std::pair<TValue, TValue>> v(size);
  for (size_t i = 0; i < size; ++i) {
    nhash::DCombineH(h1, i);
    nhash::DCombineH(h2, i);
    v[i] = std::make_pair(h1, h2);
  }
  return v;
}

template <class TValue = size_t>
constexpr std::vector<std::pair<TValue, TValue>> HRandomPair(
    size_t size, size_t seed, const TValue& max_value) {
  size_t h1 = 0, h2 = seed;
  if (h1 == h2) nhash::DCombineH(h2, h1);
  std::vector<std::pair<TValue, TValue>> v(size);
  for (size_t i = 0; i < size; ++i) {
    nhash::DCombineH(h1, i);
    nhash::DCombineH(h2, i);
    v[i] = std::make_pair(h1 % max_value, h2 % max_value);
  }
  return v;
}
}  // namespace nvector
