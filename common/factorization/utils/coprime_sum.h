#pragma once

#include "common/factorization/base.h"

// Return sum of coprime to k numbers in the interval [1..n].
constexpr uint64_t CoprimeSum(const TFactorization& k_factorization,
                              uint64_t n) {
  if (n <= 1) return n;
  if (k_factorization.size() == 0) return (n * (n + 1)) / 2;
  const uint64_t fs = k_factorization.size(), p2 = 1ull << fs;
  uint64_t total = 0;
  for (uint64_t i = 0; i < p2; ++i) {
    uint64_t tn = n, tm = 1;
    unsigned bits = 0;
    for (uint64_t j = 0; j < fs; ++j) {
      if ((1ull << j) & i) {
        ++bits;
        tn /= k_factorization[j].prime;
        tm *= k_factorization[j].prime;
      }
    }
    uint64_t t = tm * ((tn * (tn + 1)) / 2);
    total = ((bits & 1) ? total - t : total + t);
  }
  return total;
}
