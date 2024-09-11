#pragma once

#include "common/base.h"
#include "common/hash/base.h"

namespace nhash {

/**
 * @brief Computes the hash value for an object using the specified policy.
 *
 * This function computes the hash value for an object of type T using the
 * specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the object to be hashed.
 * @param x The object to be hashed.
 * @return The computed hash value.
 */
template <class Policy, class T>
constexpr size_t HValue(const T& x) {
  return Hash<Policy, T>{}(x);
}

}  // namespace nhash
