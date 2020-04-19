#pragma once

#include "common/base.h"
#include "common/factorization/table/primes.h"

#include <vector>

namespace factorization {
namespace table {
class Mobius : public Primes {
 protected:
  std::vector<int> mobius;

 public:
  explicit Mobius(uint64_t size) : Primes(size) {
    mobius.resize(Primes::table_size + 1);
    mobius[0] = 0;
    mobius[1] = 1;
    for (uint64_t i = 2; i <= Primes::table_size; ++i) {
      unsigned p = Primes::table[i];
      mobius[i] = (Primes::table[i / p] == p ? 0 : -1 * mobius[i / p]);
    }
  }

  int Get(uint64_t n) const { return mobius[n]; }
  int operator()(uint64_t n) const { return Get(n); }
};
}  // namespace table
}  // namespace factorization
