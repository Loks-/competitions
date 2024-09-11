#pragma once

#include <algorithm>

/**
 * @brief Finds the lower bound using a custom function.
 *
 * @tparam TValue1 The type of the iterator.
 * @tparam TValue2 The type of the value to compare.
 * @tparam TFunction The type of the custom function.
 * @param begin The beginning of the range.
 * @param end The end of the range.
 * @param value The value to compare.
 * @param f The custom function.
 * @return The iterator pointing to the lower bound.
 */
template <class TValue1, class TValue2, class TFunction>
constexpr TValue1 LowerBoundF(TValue1 begin, TValue1 end, const TValue2& value,
                              TFunction& f) {
  for (; begin < end;) {
    const TValue1 m = (begin + end) / 2;
    if (f(m) < value) {
      begin = m + 1;
    } else {
      end = m;
    }
  }
  return begin;
}

/**
 * @brief Finds the upper bound using a custom function.
 *
 * @tparam TValue1 The type of the iterator.
 * @tparam TValue2 The type of the value to compare.
 * @tparam TFunction The type of the custom function.
 * @param begin The beginning of the range.
 * @param end The end of the range.
 * @param value The value to compare.
 * @param f The custom function.
 * @return The iterator pointing to the upper bound.
 */
template <class TValue1, class TValue2, class TFunction>
constexpr TValue1 UpperBoundF(TValue1 begin, TValue1 end, const TValue2& value,
                              TFunction& f) {
  for (; begin < end;) {
    const TValue1 m = (begin + end) / 2;
    if (f(m) <= value) {
      begin = m + 1;
    } else {
      end = m;
    }
  }
  return begin;
}

template <class TValue1, class TFunction>
constexpr TValue1 LowerBoundB(TValue1 begin, TValue1 end, TFunction& f) {
  for (; begin < end;) {
    const TValue1 m = (begin + end) / 2;
    if (f(m)) {
      end = m;
    } else {
      begin = m + 1;
    }
  }
  return begin;
}

template <class TValue1, class TFunction>
constexpr TValue1 LowerBoundB(TValue1 begin, TFunction& f) {
  auto end = std::max(begin, TValue1(1));
  for (; !f(end); end *= 2) begin = end + 1;
  return LowerBoundB(begin, end, f);
}
