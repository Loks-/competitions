#pragma once

#include "common/vector/unique.h"

#include <algorithm>
#include <vector>

namespace nvector {

/**
 * @brief Computes the size of the union of two sorted vectors.
 *
 * This function calculates the number of unique elements that are present in
 * either of the input vectors. The input vectors must be sorted.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first sorted vector.
 * @param v2 The second sorted vector.
 * @return The number of unique elements in the union of the two vectors.
 */
template <class T>
constexpr unsigned Union(const std::vector<T>& v1, const std::vector<T>& v2) {
  unsigned r = 0, i1 = 0, i2 = 0;
  for (; (i1 < v1.size()) && (i2 < v2.size()); ++r) {
    if (v1[i1] < v2[i2]) {
      ++i1;
    } else if (v2[i2] < v1[i1]) {
      ++i2;
    } else {
      ++i1;
      ++i2;
    }
  }
  return r + (v1.size() - i1) + (v2.size() - i2);
}

/**
 * @brief Computes the union of two sorted vectors.
 *
 * This function returns a vector containing the unique elements that are
 * present in either of the input vectors. The input vectors must be sorted.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first sorted vector.
 * @param v2 The second sorted vector.
 * @return A vector containing the unique elements in the union of the two
 * vectors.
 */
template <class T>
constexpr std::vector<T> UnionV(const std::vector<T>& v1,
                                const std::vector<T>& v2) {
  std::vector<T> r;
  unsigned i1 = 0, i2 = 0;
  for (; (i1 < v1.size()) && (i2 < v2.size());) {
    if (v1[i1] < v2[i2]) {
      r.push_back(v1[i1++]);
    } else if (v2[i2] < v1[i1]) {
      r.push_back(v2[i2++]);
    } else {
      r.push_back(v1[i1++]);
      ++i2;
    }
  }
  r.insert(r.end(), v1.begin() + i1, v1.end());
  r.insert(r.end(), v2.begin() + i2, v2.end());
  return r;
}

/**
 * @brief Computes the size of the union of two unsorted vectors.
 *
 * This function sorts the input vectors and then calculates the number of
 * unique elements that are present in either of the vectors.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first unsorted vector.
 * @param v2 The second unsorted vector.
 * @return The number of unique elements in the union of the two vectors.
 */
template <class T>
constexpr unsigned UnionUnsorted(std::vector<T>& v1, std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return Union(v1, v2);
}

/**
 * @brief Computes the union of two unsorted vectors.
 *
 * This function creates a new temporary vector by concatenating the input
 * vectors and then returns a vector containing the unique elements that are
 * present in either of the vectors.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first unsorted vector.
 * @param v2 The second unsorted vector.
 * @return A vector containing the unique elements in the union of the two
 * vectors.
 */
template <class T>
constexpr std::vector<T> UnionVUnsorted(const std::vector<T>& v1,
                                        const std::vector<T>& v2) {
  auto v = v1;
  v.insert(v.end(), v2.begin(), v2.end());
  return UniqueUnsorted(v);
}

}  // namespace nvector
