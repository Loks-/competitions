#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/combine.h"
#include "common/hash/mix.h"
#include "common/hash/policy.h"
#include "common/template.h"
#include "common/templates/bits.h"

namespace nhash {
template <class Policy, class T, unsigned bits>
class HashIntegral;

template <class Policy, class T>
class HashIntegral<Policy, T, 32> {
 protected:
  static constexpr size_t HValueI(
      size_t x, TFakeIntegralTypes<EPolicyIntegralTypes::IDENTITY>) {
    return x;
  }

  static constexpr size_t HValueI(
      size_t x, TFakeIntegralTypes<EPolicyIntegralTypes::MIX>) {
    return Mix(x);
  }

 public:
  constexpr size_t operator()(T x) const {
    return HValueI(static_cast<size_t>(x),
                   TFakeIntegralTypes<Policy::integral_types>{});
  }
};

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

template <class Policy, class T>
class HashIntegral<Policy, T, 64> {
 protected:
  static constexpr size_t HValueI(
      size_t x, TFakeIntegralTypes<EPolicyIntegralTypes::IDENTITY>) {
    return x;
  }

  static constexpr size_t HValueI(
      size_t x, TFakeIntegralTypes<EPolicyIntegralTypes::MIX>) {
    return Mix(x);
  }

 public:
  constexpr size_t operator()(T x) const {
    return HValueI(static_cast<size_t>(x),
                   TFakeIntegralTypes<Policy::integral_types>{});
  }
};

template <class Policy>
class Hash<Policy, int8_t>
    : public HashIntegral<Policy, int8_t, templates::SizeInBits<size_t>()> {};

template <class Policy>
class Hash<Policy, uint8_t>
    : public HashIntegral<Policy, uint8_t, templates::SizeInBits<size_t>()> {};

template <class Policy>
class Hash<Policy, int16_t>
    : public HashIntegral<Policy, int16_t, templates::SizeInBits<size_t>()> {};

template <class Policy>
class Hash<Policy, uint16_t>
    : public HashIntegral<Policy, uint16_t, templates::SizeInBits<size_t>()> {};

template <class Policy>
class Hash<Policy, int32_t>
    : public HashIntegral<Policy, int32_t, templates::SizeInBits<size_t>()> {};

template <class Policy>
class Hash<Policy, uint32_t>
    : public HashIntegral<Policy, uint32_t, templates::SizeInBits<size_t>()> {};

template <class Policy>
class Hash<Policy, int64_t>
    : public HashIntegral<Policy, int64_t, templates::SizeInBits<size_t>()> {};

template <class Policy>
class Hash<Policy, uint64_t>
    : public HashIntegral<Policy, uint64_t, templates::SizeInBits<size_t>()> {};
}  // namespace nhash
