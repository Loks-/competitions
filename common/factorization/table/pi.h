#pragma once

#include "common/base.h"
#include "common/factorization/table/primes.h"

#include <vector>

namespace factorization {
namespace table {
class Pi : public Primes {
 protected:
  std::vector<unsigned> pi;

 public:
  constexpr explicit Pi(uint64_t size) : Primes(size) {
    pi.resize(Primes::table_size + 1);
    pi[0] = 0;
    for (uint64_t i = 1; i <= Primes::table_size; ++i)
      pi[i] = pi[i - 1] + (Primes::IsPrime(i) ? 1 : 0);
  }

  constexpr unsigned Get(uint64_t n) const { return pi[n]; }

  constexpr unsigned operator()(uint64_t n) const { return Get(n); }
};
}  // namespace table
}  // namespace factorization
