#pragma once

#include "common/base.h"

#include <random>
#include <vector>

namespace nvector {

/**
 * @brief Generates a vector of random values based on a seed.
 *
 * This function creates a std::vector of random values using a seed and the
 * default random number generator. It is useful for generating
 * non-deterministic pseudo-random values.
 *
 * @tparam TValue The type of the values in the vector. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the random number generator.
 * @return A std::vector of random values.
 */
template <class TValue = size_t>
std::vector<TValue> Random(size_t size, size_t seed) {
  std::mt19937_64 generator(seed);
  std::uniform_int_distribution<TValue> distribution;
  std::vector<TValue> v(size);
  for (auto& value : v) value = distribution(generator);
  return v;
}

/**
 * @brief Generates a vector of random values based on a seed, with a maximum
 * value.
 *
 * This function creates a std::vector of random values using a seed and the
 * default random number generator, and ensures that each value is less than the
 * specified maximum value.
 *
 * @tparam TValue The type of the values in the vector. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the random number generator.
 * @param max_value The maximum value for the random values.
 * @return A std::vector of random values, each less than max_value.
 */
template <class TValue = size_t>
std::vector<TValue> Random(size_t size, size_t seed, size_t max_value) {
  std::mt19937_64 generator(seed);
  std::uniform_int_distribution<TValue> distribution(0, max_value - 1);
  std::vector<TValue> v(size);
  for (auto& value : v) value = distribution(generator);
  return v;
}

}  // namespace nvector
