#pragma once

#include "common/factorization/table/primes.h"
#include <vector>

namespace factorization {
namespace table {
class Mobius : public Primes {
 protected:
  std::vector<int> mobius;

 public:
  Mobius(uint64_t size) : Primes(size) {
    mobius.resize(Primes::table_size + 1);
    mobius[0] = 0;
    mobius[1] = 1;
    for (unsigned i = 2; i <= Primes::table_size; ++i) {
      unsigned p = Primes::table[i];
      mobius[i] = (Primes::table[i / p] == p ? 0 : -1 * mobius[i / p]);
    }
  }

  //   int GetTable(uint64_t n) const {
  //     assert(n <= Primes::table_size);
  //     unsigned primes_count = 0;
  //     for (; n > 1;) {
  //       unsigned p = Primes::table[n];
  //       n /= p;
  //       if (Primes::table[n] == p) return 0;
  //       ++primes_count;
  //     }
  //     return (primes_count & 1) ? -1 : 1;
  //   }

  //   int Get(uint64_t n) const {
  //     if (n < mobius.size()) return mobius[n];
  //     if (n <= Primes::table_size) return GetTable(n);
  //     unsigned primes_count = 0;
  //     for (uint64_t p : Primes::primes) {
  //       if (n < p * p) break;
  //       if ((n % p) == 0) {
  //         n /= p;
  //         if ((n % p) == 0) return 0;
  //       }
  //     }
  //     assert(Primes::squared_table_size > n);
  //     if (n != 1) ++primes_count;
  //     return (primes_count & 1) ? -1 : 1;
  //   }

  int Get(uint64_t n) const { return mobius[n]; }
  int operator()(uint64_t n) const { return Get(n); }
};
}  // namespace table
}  // namespace factorization
