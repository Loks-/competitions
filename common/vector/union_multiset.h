#pragma once

#include <algorithm>
#include <vector>

namespace nvector {

/**
 * @brief Computes the size of the union of two sorted multisets.
 *
 * This function calculates the number of elements in the union of two sorted
 * multisets. Each element appears in the union the maximum number of times it
 * appears in either of the input multisets.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first sorted multiset.
 * @param v2 The second sorted multiset.
 * @return The number of elements in the union of the two multisets.
 */
template <class T>
constexpr unsigned UnionMultiset(const std::vector<T>& v1,
                                 const std::vector<T>& v2) {
  unsigned r = 0, i1 = 0, i2 = 0;
  for (; i1 < v1.size() && i2 < v2.size(); ++r) {
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
 * @brief Computes the union of two sorted multisets.
 *
 * This function returns a vector containing the elements in the union of two
 * sorted multisets. Each element appears in the union the maximum number of
 * times it appears in either of the input multisets.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first sorted multiset.
 * @param v2 The second sorted multiset.
 * @return A vector containing the elements in the union of the two multisets.
 */
template <class T>
constexpr std::vector<T> UnionMultisetV(const std::vector<T>& v1,
                                        const std::vector<T>& v2) {
  if (v1.empty()) return v2;
  if (v2.empty()) return v1;
  std::vector<T> r;
  r.reserve(v1.size() + v2.size());
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
 * @brief Computes the size of the union of two unsorted multisets.
 *
 * This function sorts the input multisets and then calculates the number of
 * elements in the union of the two multisets.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first unsorted multiset.
 * @param v2 The second unsorted multiset.
 * @return The number of elements in the union of the two multisets.
 */
template <class T>
constexpr unsigned UnionMultisetUnsorted(std::vector<T>& v1,
                                         std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return UnionMultiset(v1, v2);
}

/**
 * @brief Computes the union of two unsorted multisets.
 *
 * This function sorts the input multisets and then returns a vector containing
 * the elements in the union of the two multisets. Each element appears in the
 * union the maximum number of times it appears in either of the input
 * multisets.
 *
 * @tparam T The type of the elements in the vectors.
 * @param v1 The first unsorted multiset.
 * @param v2 The second unsorted multiset.
 * @return A vector containing the elements in the union of the two multisets.
 */
template <class T>
constexpr std::vector<T> UnionMultisetVUnsorted(std::vector<T>& v1,
                                                std::vector<T>& v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return UnionMultisetV(v1, v2);
}

}  // namespace nvector
