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
 * @tparam Iterator The type of the iterator.
 * @tparam Value The type of the value to compare.
 * @tparam Comparator The type of the comparator function.
 * @param begin The beginning of the range to search.
 * @param end The end of the range to search.
 * @param value The value to compare.
 * @param comparator The comparator function to apply to each element.
 * @return The iterator to the first element that is not less than the given
 * value, or end if no such element is found.
 */
template <typename Iterator, typename Value, typename Comparator>
[[nodiscard]] constexpr Iterator lower_bound_with_comparator(
    Iterator begin, Iterator end, const Value& value,
    const Comparator& comparator) {
  while (begin != end) {
    const Iterator mid = (begin + end) / 2;
    if (comparator(mid) < value) {
      begin = mid + 1;
    } else {
      end = mid;
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
 * @tparam Iterator The type of the iterator.
 * @tparam Value The type of the value to compare.
 * @tparam Comparator The type of the comparator function.
 * @param begin The beginning of the range to search.
 * @param end The end of the range to search.
 * @param value The value to compare.
 * @param comparator The comparator function to apply to each element.
 * @return The iterator to the first element that is greater than the given
 * value, or end if no such element is found.
 */
template <typename Iterator, typename Value, typename Comparator>
[[nodiscard]] constexpr Iterator upper_bound_with_comparator(
    Iterator begin, Iterator end, const Value& value,
    const Comparator& comparator) {
  while (begin != end) {
    const Iterator mid = (begin + end) / 2;
    if (comparator(mid) <= value) {
      begin = mid + 1;
    } else {
      end = mid;
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
 * @tparam Iterator The type of the iterator.
 * @tparam Predicate The type of the predicate function.
 * @param begin The beginning of the range to search.
 * @param end The end of the range to search.
 * @param predicate The predicate function to apply to each element.
 * @return The iterator to the first element that satisfies the predicate, or
 * end if no such element is found.
 */
template <typename Iterator, typename Predicate>
[[nodiscard]] constexpr Iterator lower_bound_with_predicate(
    Iterator begin, Iterator end, const Predicate& predicate) {
  while (begin != end) {
    const Iterator mid = (begin + end) / 2;
    if (predicate(mid)) {
      end = mid;
    } else {
      begin = mid + 1;
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
 * @tparam Iterator The type of the iterator.
 * @tparam Predicate The type of the predicate function.
 * @param begin The beginning of the range to search.
 * @param predicate The predicate function to apply to each element.
 * @return The iterator to the first element that satisfies the predicate.
 */
template <typename Iterator, typename Predicate>
[[nodiscard]] constexpr Iterator lower_bound_with_predicate(
    Iterator begin, const Predicate& predicate) {
  auto end = std::max(begin, Iterator(1));
  while (!predicate(end)) {
    begin = end + 1;
    end *= 2;
  }
  return lower_bound_with_predicate(begin, end, predicate);
}
