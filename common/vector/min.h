#pragma once

#include "common/base.h"

#include <vector>

namespace nvector {

/**
 * @brief Computes the minimum element in a vector.
 *
 * This function calculates the minimum element in the input vector.
 *
 * @tparam T The type of the elements in the vector.
 * @param v The input vector.
 * @return The minimum element in the vector.
 * @note The input vector must not be empty.
 */
template <class T>
constexpr T Min(const std::vector<T>& v) {
  assert(!v.empty() && "The input vector must not be empty.");

  T min_value = v[0];
  for (unsigned i = 1; i < v.size(); ++i) {
    if (v[i] < min_value) {
      min_value = v[i];
    }
  }
  return min_value;
}

}  // namespace nvector
