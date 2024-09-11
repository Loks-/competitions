#pragma once

#include "common/base.h"
#include "common/hash/combine.h"
#include "common/hash/policy.h"

#include <iterator>

namespace nhash {

/**
 * @brief Computes the hash value for a range of elements.
 *
 * This function computes the hash value for a range of elements specified by
 * the begin and end iterators using the specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the iterator.
 * @param begin The iterator pointing to the beginning of the range.
 * @param end The iterator pointing to the end of the range.
 * @return The computed hash value for the range. Output zero if the range is
 * empty.
 */
template <class Policy, class T>
constexpr size_t HValueRange(const T& begin, const T& end) {
  size_t seed = 0;
  for (auto it = begin; it != end;)
    CombineV<TProxyHelperITI<Policy>,
             typename std::iterator_traits<T>::value_type>(seed, *it++);
  return seed;
}

}  // namespace nhash
