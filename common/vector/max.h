#pragma once

#include "common/base.h"

#include <vector>

namespace nvector {

/**
 * @brief Computes the maximum element in a vector.
 *
 * This function calculates the maximum element in the input vector.
 *
 * @tparam T The type of the elements in the vector.
 * @param v The input vector.
 * @return The maximum element in the vector.
 * @note The input vector must not be empty.
 */
template <class T>
constexpr T Max(const std::vector<T>& v) {
  assert(!v.empty() && "The input vector must not be empty.");

  T max_value = v[0];
  for (unsigned i = 1; i < v.size(); ++i) {
    if (max_value < v[i]) {
      max_value = v[i];
    }
  }
  return max_value;
}

}  // namespace nvector
