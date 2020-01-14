#pragma once

#include "common/factorization/table/primes.h"
#include <vector>

namespace factorization {
namespace table {
class Pi : public Primes {
 protected:
  std::vector<unsigned> pi;

 public:
  Pi(uint64_t size) : Primes(size) {
    pi.resize(Primes::table_size + 1);
    pi[0] = 0;
    for (unsigned i = 1; i <= Primes::table_size; ++i)
      pi[i] = pi[i - 1] + (Primes::IsPrime(i) ? 1 : 0);
  }

  int Get(uint64_t n) const { return pi[n]; }
  int operator()(uint64_t n) const { return Get(n); }
};
}  // namespace table
}  // namespace factorization
