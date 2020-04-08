#pragma once

#include "common/base.h"
#include "common/factorization/table/primes.h"

#include <vector>

namespace factorization {
namespace table {
class EulersTotient : public Primes {
 protected:
  std::vector<unsigned> eulers_totient;

 public:
  EulersTotient(uint64_t size) : Primes(size) {
    eulers_totient.resize(Primes::table_size + 1);
    eulers_totient[0] = 0;
    eulers_totient[1] = 1;
    for (uint64_t i = 2; i <= Primes::table_size; ++i) {
      unsigned p = Primes::table[i];
      eulers_totient[i] =
          eulers_totient[i / p] * (Primes::table[i / p] == p ? p : (p - 1));
    }
  }

  uint64_t Get(uint64_t n) const { return eulers_totient[n]; }
  uint64_t operator()(uint64_t n) const { return Get(n); }
};
}  // namespace table
}  // namespace factorization
