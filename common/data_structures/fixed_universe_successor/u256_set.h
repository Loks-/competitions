#pragma once

#include "common/base.h"
#include "common/data_structures/fixed_universe_successor/empty.h"
#include "common/numeric/bits/bits_count.h"
#include "common/numeric/bits/first_bit.h"

#include <array>

namespace ds {
namespace fus {
// U = 256
class U256Set {
 public:
  static const unsigned nbits = 8;
  static const unsigned length = 4;

 protected:
  std::array<uint64_t, 4> vmask;

 public:
  U256Set() { Clear(); }

  void Clear() {
    for (auto& mask : vmask) mask = 0;
  }

  void Init(size_t) { Clear(); }

  void Set1(size_t x) {
    Clear();
    vmask[x >> 6] = (1ull << (x & 63));
  }

  // Assume mask == 0.
  void Set1I(size_t x) { vmask[x >> 6] = (1ull << (x & 63)); }

  void Insert(size_t x) { vmask[x >> 6] |= (1ull << (x & 63)); }
  bool HasKey(size_t x) const { return (vmask[x >> 6] >> (x & 63)) & 1ull; }
  void Delete(size_t x) { vmask[x >> 6] &= ~(1ull << (x & 63)); }

  size_t IsEmpty() const {
    for (auto& mask : vmask) {
      if (mask) return false;
    }
    return true;
  }

  size_t Size() const {
    size_t s = 0;
    for (auto& mask : vmask) s += numeric::BitsCount(mask);
    return s;
  }

  size_t USize() const { return 256; }

  size_t Min() const {
    for (unsigned i = 0; i < length; ++i) {
      if (vmask[i]) return (i << 6) + numeric::Lowest0Bits(vmask[i]);
    }
    return Empty;
  }

  size_t Max() const {
    for (unsigned i = length; i > 0;) {
      auto mask = vmask[--i];
      if (mask) return (i << 6) + 63 - numeric::Highest0Bits(mask);
    }
    return Empty;
  }

  // Support extra function from U64Set class.
  size_t MinI() const { return Min(); }
  size_t MaxI() const { return Max(); }

  size_t Successor(size_t x) const {
    unsigned i = (x >> 6);
    uint64_t hmask = vmask[i] & ~((2ull << (x & 63)) - 1);
    if (hmask) return (i << 6) + numeric::Lowest0Bits(hmask);
    for (++i; i < length; ++i) {
      if (vmask[i]) return (i << 6) + numeric::Lowest0Bits(vmask[i]);
    }
    return Empty;
  }

  size_t Predecessor(size_t x) const {
    unsigned i = (x >> 6);
    uint64_t lmask = vmask[i] & ((1ull << (x & 63)) - 1);
    if (lmask) return (i << 6) + numeric::Highest0Bits(lmask);
    for (--i; i < length; --i) {
      if (vmask[i]) return (i << 6) + numeric::Highest0Bits(vmask[i]);
    }
    return Empty;
  }
};
}  // namespace fus
}  // namespace ds
