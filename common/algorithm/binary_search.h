#pragma once

#include <algorithm>

/**
 * @brief Finds the first element in the range [begin, end) that is not less
 * than the given value using a custom comparator.
 *
 * This function performs a binary search from begin to end, applying the
 * comparator f to each element. If an element is not less than the given value,
 * the search stops and the iterator to that element is returned. If no such
 * element is found, the end iterator is returned.
 *
 * The comparator function f should be monotonic.
 *
 * @tparam TIterator The type of the iterator.
 * @tparam TValue The type of the value to compare.
 * @tparam TFunction The type of the comparator function.
 * @param begin The beginning of the range to search.
 * @param end The end of the range to search.
 * @param value The value to compare.
 * @param f The comparator function to apply to each element.
 * @return The iterator to the first element that is not less than the given
 * value, or end if no such element is found.
 */
template <class TIterator, class TValue, class TFunction>
constexpr TIterator LowerBoundF(TIterator begin, TIterator end,
                                const TValue& value, const TFunction& f) {
  for (; begin < end;) {
    const TIterator m = (begin + end) / 2;
    if (f(m) < value) {
      begin = m + 1;
    } else {
      end = m;
    }
  }
  return begin;
}

/**
 * @brief Finds the first element in the range [begin, end) that is greater than
 * the given value using a custom comparator.
 *
 * This function performs a binary search from begin to end, applying the
 * comparator f to each element. If an element is greater than the given value,
 * the search stops and the iterator to that element is returned. If no such
 * element is found, the end iterator is returned.
 *
 * The comparator function f should be monotonic.
 *
 * @tparam TIterator The type of the iterator.
 * @tparam TValue The type of the value to compare.
 * @tparam TFunction The type of the comparator function.
 * @param begin The beginning of the range to search.
 * @param end The end of the range to search.
 * @param value The value to compare.
 * @param f The comparator function to apply to each element.
 * @return The iterator to the first element that is greater than the given
 * value, or end if no such element is found.
 */
template <class TIterator, class TValue, class TFunction>
constexpr TIterator UpperBoundF(TIterator begin, TIterator end,
                                const TValue& value, const TFunction& f) {
  for (; begin < end;) {
    const TIterator m = (begin + end) / 2;
    if (f(m) <= value) {
      begin = m + 1;
    } else {
      end = m;
    }
  }
  return begin;
}

/**
 * @brief Finds the first element in the range [begin, end) that satisfies the
 * predicate f.
 *
 * This function performs a binary search from begin to end, applying the
 * predicate f to each element. If an element satisfies the predicate, the
 * search stops and the iterator to that element is returned. If no such element
 * is found, the end iterator is returned.
 *
 * The predicate function f should be monotonic.
 *
 * @tparam TIterator The type of the iterator.
 * @tparam TFunction The type of the predicate function.
 * @param begin The beginning of the range to search.
 * @param end The end of the range to search.
 * @param f The predicate function to apply to each element.
 * @return The iterator to the first element that satisfies the predicate, or
 * end if no such element is found.
 */
template <class TIterator, class TFunction>
constexpr TIterator LowerBoundB(TIterator begin, TIterator end,
                                const TFunction& f) {
  for (; begin < end;) {
    const TIterator m = (begin + end) / 2;
    if (f(m)) {
      end = m;
    } else {
      begin = m + 1;
    }
  }
  return begin;
}

/**
 * @brief Finds the first element starting from begin that satisfies the
 * predicate f.
 *
 * This function performs an unbounded binary search starting from begin,
 * applying the predicate f to each element. If an element satisfies the
 * predicate, the search stops and the iterator to that element is returned.
 *
 * The predicate function f should be monotonic.
 *
 * @tparam TIterator The type of the iterator.
 * @tparam TFunction The type of the predicate function.
 * @param begin The beginning of the range to search.
 * @param f The predicate function to apply to each element.
 * @return The iterator to the first element that satisfies the predicate.
 */
template <class TIterator, class TFunction>
constexpr TIterator LowerBoundB(TIterator begin, const TFunction& f) {
  auto end = std::max(begin, TIterator(1));
  for (; !f(end); end *= 2) begin = end + 1;
  return LowerBoundB(begin, end, f);
}
