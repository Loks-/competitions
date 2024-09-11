#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/range.h"

#include <vector>

namespace nhash {

/**
 * @brief Specialization of Hash for std::vector.
 *
 * This specialization provides a hash function for std::vector based on the
 * specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the elements in the vector.
 * @tparam TAllocator The allocator type for the vector.
 */
template <class Policy, class T, class TAllocator>
class Hash<Policy, std::vector<T, TAllocator>> {
 public:
  /**
   * @brief Computes the hash value for a std::vector.
   *
   * This function computes the hash value for a std::vector by hashing the
   * range of elements in the vector using the specified policy.
   *
   * @param v The vector to be hashed.
   * @return The computed hash value of the vector. Hash value is zero if the
   * vector is empty.
   */
  constexpr size_t operator()(const std::vector<T, TAllocator>& v) const {
    return HValueRange<Policy>(v.begin(), v.end());
  }
};

}  // namespace nhash
