#pragma once

/**
 * @brief Finds the first element in the range [begin, end) that satisfies the
 * predicate f.
 *
 * This function performs a linear search from begin to end, applying the
 * predicate f to each element. If an element satisfies the predicate, the
 * search stops and the iterator to that element is returned. If no element
 * satisfies the predicate, the end iterator is returned.
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
[[nodiscard]] constexpr Iterator find_first_if(Iterator begin,
                                               const Iterator end,
                                               Predicate&& predicate) {
  for (; begin != end; ++begin) {
    if (predicate(begin)) break;
  }
  return begin;
}

/**
 * @brief Finds the first element starting from begin that satisfies the
 * predicate f.
 *
 * This function performs an unbounded linear search starting from begin,
 * applying the predicate f to each element. If an element satisfies the
 * predicate, the search stops and the iterator to that element is returned.
 *
 * @tparam Iterator The type of the iterator.
 * @tparam Predicate The type of the predicate function.
 * @param begin The beginning of the range to search.
 * @param predicate The predicate function to apply to each element.
 * @return The iterator to the first element that satisfies the predicate.
 */
template <typename Iterator, typename Predicate>
[[nodiscard]] constexpr Iterator find_first_if(Iterator begin,
                                               Predicate&& predicate) {
  for (;; ++begin) {
    if (predicate(begin)) return begin;
  }
}
