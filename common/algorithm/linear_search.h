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
 * @tparam TIterator The type of the iterator.
 * @tparam TFunction The type of the predicate function.
 * @param begin The beginning of the range to search.
 * @param end The end of the range to search.
 * @param f The predicate function to apply to each element.
 * @return The iterator to the first element that satisfies the predicate, or
 * end if no such element is found.
 */
template <class TIterator, class TFunction>
constexpr TIterator FirstIfB(TIterator begin, const TIterator end,
                             const TFunction& f) {
  for (; begin < end; ++begin) {
    if (f(begin)) {
      break;
    }
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
 * @tparam TIterator The type of the iterator.
 * @tparam TFunction The type of the predicate function.
 * @param begin The beginning of the range to search.
 * @param f The predicate function to apply to each element.
 * @return The iterator to the first element that satisfies the predicate.
 */
template <class TIterator, class TFunction>
constexpr TIterator FirstIfB(TIterator begin, const TFunction& f) {
  for (;; ++begin) {
    if (f(begin)) {
      return begin;
    }
  }
}
