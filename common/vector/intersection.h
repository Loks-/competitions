#pragma once

#include <algorithm>
#include <vector>

namespace nvector {

/**
 * @brief Computes the size of the intersection of two sorted vectors.
 *
 * This function calculates the number of elements that are present in both
 * input vectors. The input vectors must be sorted.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first sorted vector.
 * @param v2 The second sorted vector.
 * @return The number of elements in the intersection of the two vectors.
 */
template <class T>
constexpr unsigned Intersection(const std::vector<T>& v1,
                                const std::vector<T>& v2) {
  unsigned r = 0;
  for (unsigned i1 = 0, i2 = 0; (i1 < v1.size()) && (i2 < v2.size());) {
    if (v1[i1] < v2[i2]) {
      ++i1;
    } else if (v2[i2] < v1[i1]) {
      ++i2;
    } else {
      ++r;
      ++i1;
      ++i2;
    }
  }
  return r;
}

/**
 * @brief Computes the intersection of two sorted vectors.
 *
 * This function returns a vector containing the elements that are present in
 * both input vectors. The input vectors must be sorted.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first sorted vector.
 * @param v2 The second sorted vector.
 * @return A vector containing the elements in the intersection of the two
 * vectors.
 */
template <class T>
constexpr std::vector<T> IntersectionV(const std::vector<T>& v1,
                                       const std::vector<T>& v2) {
  std::vector<T> r;
  for (unsigned i1 = 0, i2 = 0; (i1 < v1.size()) && (i2 < v2.size());) {
    if (v1[i1] < v2[i2]) {
      ++i1;
    } else if (v2[i2] < v1[i1]) {
      ++i2;
    } else {
      r.push_back(v1[i1]);
      ++i1;
      ++i2;
    }
  }
  return r;
}

/**
 * @brief Computes the size of the intersection of two unsorted vectors.
 *
 * This function sorts the input vectors and then calculates the number of
 * elements that are present in both vectors.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first unsorted vector.
 * @param v2 The second unsorted vector.
 * @return The number of elements in the intersection of the two vectors.
 */
template <class T>
constexpr unsigned IntersectionUnsorted(std::vector<T>& v1,
                                        std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return Intersection(v1, v2);
}

/**
 * @brief Computes the intersection of two unsorted vectors.
 *
 * This function sorts the input vectors and then returns a vector containing
 * the elements that are present in both vectors.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first unsorted vector.
 * @param v2 The second unsorted vector.
 * @return A vector containing the elements in the intersection of the two
 * vectors.
 */
template <class T>
constexpr std::vector<T> IntersectionVUnsorted(std::vector<T>& v1,
                                               std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return IntersectionV(v1, v2);
}

}  // namespace nvector
