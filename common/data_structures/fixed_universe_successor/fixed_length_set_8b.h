#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/numeric/bits/bits_count.h"
#include "common/numeric/bits/first_bit.h"

#include <array>

namespace ds {
namespace fus {
// U = 256
class FLSetB8 {
 public:
  static constexpr unsigned nbits = 8;
  static constexpr unsigned length = 4;

 protected:
  std::array<uint64_t, length> vmask;

 public:
  constexpr FLSetB8() { Clear(); }

  constexpr void Clear() {
    for (auto& mask : vmask) mask = 0;
  }

  constexpr void Init(size_t) { Clear(); }

  constexpr void Set1(size_t x) {
    Clear();
    vmask[x >> 6] = (1ull << (x & 63));
  }

  // Assume mask == 0.
  constexpr void Set1I(size_t x) { vmask[x >> 6] = (1ull << (x & 63)); }

  constexpr void Insert(size_t x) { vmask[x >> 6] |= (1ull << (x & 63)); }

  constexpr bool HasKey(size_t x) const {
    return (vmask[x >> 6] >> (x & 63)) & 1ull;
  }

  constexpr void Delete(size_t x) { vmask[x >> 6] &= ~(1ull << (x & 63)); }

  constexpr size_t IsEmpty() const {
    for (auto& mask : vmask) {
      if (mask) return false;
    }
    return true;
  }

  constexpr size_t Size() const {
    size_t s = 0;
    for (auto& mask : vmask) s += numeric::BitsCount(mask);
    return s;
  }

  static consteval size_t USize() { return 256; }

  constexpr size_t Min() const {
    for (unsigned i = 0; i < length; ++i) {
      if (vmask[i]) return (i << 6) + numeric::Lowest0Bits(vmask[i]);
    }
    return kEmpty;
  }

  constexpr size_t Max() const {
    for (unsigned i = length; i > 0;) {
      auto mask = vmask[--i];
      if (mask) return (i << 6) + numeric::HighestBit(mask);
    }
    return kEmpty;
  }

  // Support extra function from U64Set class.
  constexpr size_t MinI() const { return Min(); }
  constexpr size_t MaxI() const { return Max(); }

  constexpr size_t Successor(size_t x) const {
    unsigned i = (x >> 6);
    const uint64_t hmask = vmask[i] & ~((2ull << (x & 63)) - 1);
    if (hmask) return (i << 6) + numeric::Lowest0Bits(hmask);
    for (++i; i < length; ++i) {
      if (vmask[i]) return (i << 6) + numeric::Lowest0Bits(vmask[i]);
    }
    return kEmpty;
  }

  constexpr size_t Predecessor(size_t x) const {
    unsigned i = (x >> 6);
    const uint64_t lmask = vmask[i] & ((1ull << (x & 63)) - 1);
    if (lmask) return (i << 6) + numeric::HighestBit(lmask);
    for (--i; i < length; --i) {
      if (vmask[i]) return (i << 6) + numeric::HighestBit(vmask[i]);
    }
    return kEmpty;
  }
};
}  // namespace fus
}  // namespace ds
