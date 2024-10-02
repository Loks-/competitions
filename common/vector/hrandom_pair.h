#pragma once

#include "common/hash/combine.h"

#include <utility>
#include <vector>

namespace nvector {

/**
 * @brief Generates a vector of pairs of hashed values based on a seed.
 *
 * This function creates a std::vector of pairs of hashed values using a seed
 * and a hash function. It is useful for generating deterministic pseudo-random
 * pairs of values.
 *
 * @tparam TValue The type of the values in the pairs. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the hash function.
 * @return A std::vector of pairs of hashed values.
 */
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

/**
 * @brief Generates a vector of pairs of hashed values based on a seed, with a
 * maximum value.
 *
 * This function creates a std::vector of pairs of hashed values using a seed
 * and a hash function, and ensures that each value in the pairs is less than
 * the specified maximum value.
 *
 * @tparam TValue The type of the values in the pairs. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the hash function.
 * @param max_value The maximum value for the hashed values.
 * @return A std::vector of pairs of hashed values, each less than max_value.
 */
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
