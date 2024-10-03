#pragma once

#include <algorithm>
#include <vector>

namespace nvector {

/**
 * @brief Computes the length of the longest increasing subsequence in a vector.
 *
 * This function calculates the length of the longest increasing subsequence
 * in the input vector using a binary search approach. The time complexity is
 * O(n log n), where n is the size of the input vector.
 *
 * @tparam TValue The type of the elements in the vector.
 * @param v The input vector.
 * @return The length of the longest increasing subsequence.
 */
template <class TValue>
constexpr unsigned LongestIncresingSubsequence(const std::vector<TValue>& v) {
  std::vector<TValue> vd;
  for (auto& x : v) {
    auto it = std::lower_bound(vd.begin(), vd.end(), x);
    if (it == vd.end()) {
      vd.push_back(x);
    } else {
      *it = x;
    }
  }
  return vd.size();
}

}  // namespace nvector
