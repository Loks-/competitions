#pragma once

#include <vector>

namespace nvector {

/**
 * @brief Computes the product of all elements in a vector.
 *
 * This function calculates the product of all elements in the input vector.
 *
 * @tparam T The type of the elements in the vector.
 * @param v The input vector.
 * @return The product of all elements in the vector.
 */
template <class T>
constexpr T Product(const std::vector<T>& v) {
  T result(1);
  for (const auto& x : v) {
    result *= x;
  }
  return result;
}

}  // namespace nvector
