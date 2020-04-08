#pragma once

#include "common/factorization/base.h"

// Return count of coprime to k numbers in the interval [1..n].
inline uint64_t CoprimeCount(const TFactorization& k_factorization,
                             uint64_t n) {
  if (n <= 1) return n;
  if (k_factorization.size() == 0) return n;
  uint64_t fs = k_factorization.size(), p2 = 1ull << fs;
  uint64_t total = 0;
  for (uint64_t i = 0; i < p2; ++i) {
    uint64_t t = n;
    unsigned bits = 0;
    for (uint64_t j = 0; j < fs; ++j) {
      if ((1ull << j) & i) {
        ++bits;
        t /= k_factorization[j].prime;
      }
    }
    total = ((bits & 1) ? total - t : total + t);
  }
  return total;
}
