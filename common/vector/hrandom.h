#pragma once

#include "common/hash/combine.h"

#include <vector>

namespace nvector {

/**
 * @brief Generates a vector of hashed values based on a seed.
 *
 * This function creates a std::vector of hashed values using a seed and a hash
 * function. It is useful for generating deterministic pseudo-random values.
 *
 * @tparam TValue The type of the values in the vector. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the hash function.
 * @return A std::vector of hashed values.
 */
template <class TValue = size_t>
constexpr std::vector<TValue> HRandom(size_t size, size_t seed) {
  size_t h = seed;
  std::vector<TValue> v(size);
  for (size_t i = 0; i < size; ++i) {
    nhash::DCombineH(h, i);
    v[i] = TValue(h);
  }
  return v;
}

/**
 * @brief Generates a vector of hashed values based on a seed, with a maximum
 * value.
 *
 * This function creates a std::vector of hashed values using a seed and a hash
 * function, and ensures that each value is less than the specified maximum
 * value.
 *
 * @tparam TValue The type of the values in the vector. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the hash function.
 * @param max_value The maximum value for the hashed values.
 * @return A std::vector of hashed values, each less than max_value.
 */
template <class TValue = size_t>
constexpr std::vector<TValue> HRandom(size_t size, size_t seed,
                                      size_t max_value) {
  size_t h = seed;
  std::vector<TValue> v(size);
  for (size_t i = 0; i < size; ++i) {
    nhash::DCombineH(h, i);
    v[i] = TValue(h % max_value);
  }
  return v;
}

}  // namespace nvector
