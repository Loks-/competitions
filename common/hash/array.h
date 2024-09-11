#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/range.h"

#include <array>

namespace nhash {

/**
 * @brief Specialization of Hash for std::array.
 *
 * This specialization provides a hash function for std::array based on the
 * specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the elements in the array.
 * @tparam size The size of the array.
 */
template <class Policy, class T, size_t size>
class Hash<Policy, std::array<T, size>> {
 public:
  /**
   * @brief Computes the hash value for a std::array.
   *
   * This function computes the hash value for a std::array by hashing the
   * range of elements in the array using the specified policy.
   *
   * @param v The array to be hashed.
   * @return The computed hash value of the array. Hash value is zero if the
   * array is empty.
   */
  constexpr size_t operator()(const std::array<T, size>& v) const {
    return HValueRange<Policy>(v.begin(), v.end());
  }
};

}  // namespace nhash
