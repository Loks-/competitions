#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/combine.h"
#include "common/hash/mix.h"
#include "common/hash/policy.h"
#include "common/template.h"
#include "common/templates/bits.h"

namespace nhash {

/**
 * @brief Hash class template for integral types.
 *
 * This class template provides a hash function for integral types based on the
 * specified policy and bit size.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the integral value to be hashed.
 * @tparam bits The number of bits in the size_t type.
 */
template <class Policy, class T, unsigned bits>
class HashIntegral;

/**
 * @brief Specialization of HashIntegral for 32-bit size_t type.
 *
 * This specialization provides a hash function for integral types with a 32-bit
 * size_t type based on the specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the integral value to be hashed.
 */
template <class Policy, class T>
class HashIntegral<Policy, T, 32> {
 protected:
  static constexpr size_t HValueI(
      size_t x, TMetaIntegralTypes<EPolicyIntegralTypes::IDENTITY>) {
    return x;
  }

  static constexpr size_t HValueI(
      size_t x, TMetaIntegralTypes<EPolicyIntegralTypes::MIX>) {
    return Mix(x);
  }

 public:
  constexpr size_t operator()(T x) const {
    return HValueI(static_cast<size_t>(x),
                   TMetaIntegralTypes<Policy::integral_types>{});
  }
};

/**
 * @brief Specialization of HashIntegral for 32-bit size_t type and int64_t
 * values.
 *
 * This specialization provides a hash function for int64_t type with a
 * 32-bit size_t type based on the specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class HashIntegral<Policy, int64_t, 32> {
 public:
  constexpr size_t operator()(int64_t x) const {
    size_t seed = 0;
    CombineH<Policy>(seed, size_t(x));
    CombineH<Policy>(seed, size_t(x >> 32));
    return seed;
  }
};

/**
 * @brief Specialization of HashIntegral for 32-bit size_t type and uint64_t
 * values.
 *
 * This specialization provides a hash function for uint64_t type with a
 * 32-bit size_t type based on the specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class HashIntegral<Policy, uint64_t, 32> {
 public:
  constexpr size_t operator()(uint64_t x) const {
    size_t seed = 0;
    CombineH<Policy>(seed, size_t(x));
    CombineH<Policy>(seed, size_t(x >> 32));
    return seed;
  }
};

/**
 * @brief Specialization of HashIntegral for 64-bit size_t type.
 *
 * This specialization provides a hash function for integral types with a 64-bit
 * size_t type based on the specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the integral value to be hashed.
 */
template <class Policy, class T>
class HashIntegral<Policy, T, 64> {
 protected:
  static constexpr size_t HValueI(
      size_t x, TMetaIntegralTypes<EPolicyIntegralTypes::IDENTITY>) {
    return x;
  }

  static constexpr size_t HValueI(
      size_t x, TMetaIntegralTypes<EPolicyIntegralTypes::MIX>) {
    return Mix(x);
  }

 public:
  constexpr size_t operator()(T x) const {
    return HValueI(static_cast<size_t>(x),
                   TMetaIntegralTypes<Policy::integral_types>{});
  }
};

/**
 * @brief Specialization of Hash for char type.
 *
 * This specialization provides a hash function for char values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, char>
    : public HashIntegral<Policy, char, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for int8_t type.
 *
 * This specialization provides a hash function for int8_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, int8_t>
    : public HashIntegral<Policy, int8_t, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for uint8_t type.
 *
 * This specialization provides a hash function for uint8_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, uint8_t>
    : public HashIntegral<Policy, uint8_t, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for int16_t type.
 *
 * This specialization provides a hash function for int16_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, int16_t>
    : public HashIntegral<Policy, int16_t, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for uint16_t type.
 *
 * This specialization provides a hash function for uint16_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, uint16_t>
    : public HashIntegral<Policy, uint16_t, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for int32_t type.
 *
 * This specialization provides a hash function for int32_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, int32_t>
    : public HashIntegral<Policy, int32_t, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for uint32_t type.
 *
 * This specialization provides a hash function for uint32_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, uint32_t>
    : public HashIntegral<Policy, uint32_t, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for int64_t type.
 *
 * This specialization provides a hash function for int64_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, int64_t>
    : public HashIntegral<Policy, int64_t, templates::SizeInBits<size_t>()> {};

/**
 * @brief Specialization of Hash for uint64_t type.
 *
 * This specialization provides a hash function for uint64_t values.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 */
template <class Policy>
class Hash<Policy, uint64_t>
    : public HashIntegral<Policy, uint64_t, templates::SizeInBits<size_t>()> {};

}  // namespace nhash
