#pragma once

#include <utility>

namespace std {

/**
 * @brief Adds the elements of the right-hand pair to the left-hand pair.
 *
 * This operator overloads the += operator for std::pair, allowing element-wise addition.
 *
 * @tparam T1 The type of the first element of the pair.
 * @tparam T2 The type of the second element of the pair.
 * @param l The left-hand pair to which the right-hand pair will be added.
 * @param r The right-hand pair whose elements will be added to the left-hand pair.
 * @return A reference to the modified left-hand pair.
 */
template <class T1, class T2>
constexpr pair<T1, T2>& operator+=(pair<T1, T2>& l, const pair<T1, T2>& r) {
  l.first += r.first;
  l.second += r.second;
  return l;
}

/**
 * @brief Subtracts the elements of the right-hand pair from the left-hand pair.
 *
 * This operator overloads the -= operator for std::pair, allowing element-wise subtraction.
 *
 * @tparam T1 The type of the first element of the pair.
 * @tparam T2 The type of the second element of the pair.
 * @param l The left-hand pair from which the right-hand pair will be subtracted.
 * @param r The right-hand pair whose elements will be subtracted from the left-hand pair.
 * @return A reference to the modified left-hand pair.
 */
template <class T1, class T2>
constexpr pair<T1, T2>& operator-=(pair<T1, T2>& l, const pair<T1, T2>& r) {
  l.first -= r.first;
  l.second -= r.second;
  return l;
}

/**
 * @brief Returns a new pair that is the sum of the elements of the two input pairs.
 *
 * This operator overloads the + operator for std::pair, allowing element-wise addition.
 *
 * @tparam T1 The type of the first element of the pair.
 * @tparam T2 The type of the second element of the pair.
 * @param l The left-hand pair to be added.
 * @param r The right-hand pair to be added.
 * @return A new pair containing the element-wise sum of the input pairs.
 */
template <class T1, class T2>
constexpr pair<T1, T2> operator+(const pair<T1, T2>& l, const pair<T1, T2>& r) {
  return make_pair(l.first + r.first, l.second + r.second);
}

/**
 * @brief Returns a new pair that is the difference of the elements of the two input pairs.
 *
 * This operator overloads the - operator for std::pair, allowing element-wise subtraction.
 *
 * @tparam T1 The type of the first element of the pair.
 * @tparam T2 The type of the second element of the pair.
 * @param l The left-hand pair to be subtracted from.
 * @param r The right-hand pair to be subtracted.
 * @return A new pair containing the element-wise difference of the input pairs.
 */
template <class T1, class T2>
constexpr pair<T1, T2> operator-(const pair<T1, T2>& l, const pair<T1, T2>& r) {
  return make_pair(l.first - r.first, l.second - r.second);
}

}  // namespace std
