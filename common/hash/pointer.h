#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/integral.h"
#include "common/templates/bits.h"

namespace nhash {

/**
 * @brief Hash class template for pointer types.
 *
 * This class template provides a hash function for pointer types based on the
 * specified policy and bit size.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the pointer to be hashed.
 * @tparam bits The number of bits in the size_t type.
 */
template <class Policy, class T, unsigned bits>
class HashPointer;

/**
 * @brief Specialization of HashPointer for 32-bit size_t type.
 *
 * This specialization provides a hash function for pointer types with a 32-bit
 * size_t type based on the specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the pointer to be hashed.
 */
template <class Policy, class T>
class HashPointer<Policy, T, 32> {
 public:
  /**
   * @brief Hashes a pointer value.
   *
   * This function hashes the pointer value by reinterpreting it as a uint32_t.
   *
   * @param p The pointer value to be hashed.
   * @return The hash value of the pointer.
   */
  constexpr size_t operator()(const T* p) const {
    return h(reinterpret_cast<uint32_t>(p));
  }

 protected:
  Hash<Policy, uint32_t> h;  ///< Hash function for uint32_t values.
};

/**
 * @brief Specialization of HashPointer for 64-bit size_t type.
 *
 * This specialization provides a hash function for pointer types with a 64-bit
 * size_t type based on the specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the pointer to be hashed.
 */
template <class Policy, class T>
class HashPointer<Policy, T, 64> {
 public:
  /**
   * @brief Hashes a pointer value.
   *
   * This function hashes the pointer value by reinterpreting it as a uint64_t.
   *
   * @param p The pointer value to be hashed.
   * @return The hash value of the pointer.
   */
  constexpr size_t operator()(const T* p) const {
    return h(reinterpret_cast<uint64_t>(p));
  }

 protected:
  Hash<Policy, uint64_t> h;  ///< Hash function for uint64_t values.
};

/**
 * @brief Specialization of Hash for const pointer types.
 *
 * This specialization provides a hash function for const pointer types.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the pointer to be hashed.
 */
template <class Policy, class T>
class Hash<Policy, const T*>
    : public HashPointer<Policy, T, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for pointer types.
 *
 * This specialization provides a hash function for pointer types.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the pointer to be hashed.
 */
template <class Policy, class T>
class Hash<Policy, T*>
    : public HashPointer<Policy, T, templates::SizeInBits<size_t>()> {};

}  // namespace nhash
