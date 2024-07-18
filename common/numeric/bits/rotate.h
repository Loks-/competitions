#pragma once

#include "common/base.h"

namespace numeric {
inline uint64_t RotateBitsR(uint64_t x, unsigned shift) {
  shift &= 63;
  if (shift) x = ((((1ull << shift) - 1) & x) << (64 - shift)) | (x >> shift);
  return x;
}

inline uint64_t RotateBitsL(uint64_t x, unsigned shift) {
  return RotateBitsR(x, 64 - (shift & 63));
}

inline uint64_t RotateBitsR(uint64_t x, unsigned shift, unsigned length) {
  shift %= length;
  uint64_t tail = x & ((1ull << length) - 1);
  if (shift)
    x = ((((1ull << shift) - 1) & tail) << (length - shift)) | (tail >> shift) |
        (x ^ tail);
  return x;
}

inline uint64_t RotateBitsL(uint64_t x, unsigned shift, unsigned length) {
  return RotateBitsR(x, length - (shift % length), length);
}
}  // namespace numeric
