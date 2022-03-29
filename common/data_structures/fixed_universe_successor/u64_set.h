#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/numeric/bits/bits_count.h"
#include "common/numeric/bits/first_bit.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace fus {
// U = 64
class U64Set {
 protected:
  uint64_t mask;

 public:
  void Clear() { mask = 0; }
  void Init(size_t) { mask = 0; }
  void Insert(size_t x) { mask |= (1ull << x); }
  bool HasKey(size_t x) const { return (mask >> x) & 1ull; }
  void Delete(size_t x) { mask &= ~(1ull << x); }

  size_t Size() const { return numeric::BitsCount(mask); }
  size_t USize() const { return 64; }

  size_t Min() const {
    auto p = numeric::FirstBit(mask);
    return p ? p - 1 : Empty;
  }

  size_t Max() const { return mask ? 63 - numeric::Highest0Bits(mask) : Empty; }

  size_t Successor(size_t x) const {
    if (x == 63) return Empty;
    auto p = numeric::FirstBit(mask >> (x + 1));
    return p ? p + x : Empty;
  }

  size_t Predecessor(size_t x) const {
    uint64_t t = mask & ((1ull << x) - 1ull);
    return t ? 63 - numeric::Highest0Bits(t) : Empty;
  }
};
}  // namespace fus
}  // namespace ds
