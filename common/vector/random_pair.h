#pragma once

#include "common/base.h"

#include <random>
#include <utility>
#include <vector>

namespace nvector {

/**
 * @brief Generates a vector of pairs of random values based on a seed.
 *
 * This function creates a std::vector of pairs of random values using a seed
 * and the default random number generator. It is useful for generating
 * non-deterministic pseudo-random pairs of values.
 *
 * @tparam TValue The type of the values in the pairs. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the random number generator.
 * @return A std::vector of pairs of random values.
 */
template <class TValue = size_t>
std::vector<std::pair<TValue, TValue>> RandomPair(size_t size, size_t seed) {
  std::mt19937_64 generator(seed);
  std::uniform_int_distribution<TValue> distribution;
  std::vector<std::pair<TValue, TValue>> v(size);
  for (auto& value : v) {
    value = std::make_pair(distribution(generator), distribution(generator));
  }
  return v;
}

/**
 * @brief Generates a vector of pairs of random values based on a seed, with a
 * maximum value.
 *
 * This function creates a std::vector of pairs of random values using a seed
 * and the default random number generator, and ensures that each value in the
 * pairs is less than the specified maximum value.
 *
 * @tparam TValue The type of the values in the pairs. Default is size_t.
 * @param size The number of elements in the vector.
 * @param seed The seed value for the random number generator.
 * @param max_value The maximum value for the random values.
 * @return A std::vector of pairs of random values, each less than max_value.
 */
template <class TValue = size_t>
std::vector<std::pair<TValue, TValue>> RandomPair(size_t size, size_t seed,
                                                  size_t max_value) {
  std::mt19937_64 generator(seed);
  std::uniform_int_distribution<TValue> distribution(0, max_value - 1);
  std::vector<std::pair<TValue, TValue>> v(size);
  for (auto& value : v) {
    value = std::make_pair(distribution(generator), distribution(generator));
  }
  return v;
}

}  // namespace nvector
