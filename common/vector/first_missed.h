#pragma once

#include <algorithm>
#include <vector>

namespace nvector {

/**
 * @brief Finds the smallest non-negative integer not present in the vector.
 *
 * This function sorts the input vector and then finds the smallest non-negative
 * integer that is not present in the vector. It is useful for calculating
 * Grundy/Nim values.
 *
 * @param v The input vector of unsigned integers.
 * @return The smallest non-negative integer not present in the vector.
 */
constexpr unsigned FirstMissed(std::vector<unsigned>& v) {
  std::sort(v.begin(), v.end());
  unsigned k = 0, l = 0;
  for (; (l < v.size()) && (v[l] <= k); ++l) {
    if (v[l] == k) {
      ++k;
    }
  }
  return k;
}

}  // namespace nvector
