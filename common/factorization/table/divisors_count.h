#pragma once

#include "common/factorization/table/primes.h"
#include <vector>

namespace factorization {
namespace table {
class DivisorsCount : public Primes {
 protected:
  std::vector<unsigned> dc;

 public:
  DivisorsCount(uint64_t size) : Primes(size) {
    dc.resize(Primes::table_size + 1, 1);
    for (uint64_t i = 1; i <= Primes::table_size; ++i) {
      if (!Primes::IsPrime(i)) continue;
      uint64_t k = i;
      unsigned ip = 1;
      for (; k <= Primes::table_size; k *= i, ++ip) {
        for (uint64_t l = k; l <= Primes::table_size; l += k)
          dc[l] = (ip + 1) * (dc[l] / ip);
      }
    }
  }

  int Get(uint64_t n) const { return dc[n]; }
  int operator()(uint64_t n) const { return Get(n); }
};
}  // namespace table
}  // namespace factorization
