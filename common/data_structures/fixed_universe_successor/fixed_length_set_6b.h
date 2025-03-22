#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/numeric/bits/bits_count.h"
#include "common/numeric/bits/first_bit.h"

namespace ds {
namespace fus {
// U = 64
class FLSetB6 {
 public:
  static constexpr unsigned nbits = 6;

 protected:
  uint64_t mask;

 public:
  constexpr FLSetB6() { Clear(); }
  constexpr void Clear() { mask = 0; }
  constexpr void Init(size_t) { mask = 0; }
  constexpr void Set1(size_t x) { mask = (1ull << x); }
  constexpr void Insert(size_t x) { mask |= (1ull << x); }
  constexpr bool HasKey(size_t x) const { return (mask >> x) & 1ull; }
  constexpr void Delete(size_t x) { mask &= ~(1ull << x); }

  constexpr size_t IsEmpty() const { return (mask == 0ull); }
  constexpr size_t Size() const { return numeric::BitsCount(mask); }
  static consteval size_t USize() { return 64; }

  constexpr size_t Min() const {
    auto p = numeric::FirstBit(mask);
    return p ? p - 1 : kEmpty;
  }

  constexpr size_t Max() const {
    return mask ? numeric::HighestBit(mask) : kEmpty;
  }

  constexpr size_t Successor(size_t x) const {
    if (x == 63) return kEmpty;
    const auto p = numeric::FirstBit(mask >> (x + 1));
    return p ? p + x : kEmpty;
  }

  constexpr size_t Predecessor(size_t x) const {
    const uint64_t t = mask & ((1ull << x) - 1ull);
    return t ? numeric::HighestBit(t) : kEmpty;
  }

 public:
  // Assume mask != 0.
  constexpr size_t MinI() const { return numeric::Lowest0Bits(mask); }

  constexpr size_t MaxI() const { return numeric::HighestBit(mask); }

  // Assume mask == 0.
  constexpr void Set1I(size_t x) { Set1(x); }
};
}  // namespace fus
}  // namespace ds
