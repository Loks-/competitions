#pragma once

#include "common/factorization/primes_list.h"
#include <vector>

namespace factorization {
namespace ple {
class Mobius : public PrimesList {
 protected:
  std::vector<int> mobius;

 public:
  Mobius(uint64_t size) : PrimesList(size) {}

  void Precalc() {
    mobius.resize(PrimesList::table_size + 1);
    mobius[0] = 0;
    mobius[1] = 1;
    for (unsigned i = 2; i <= PrimesList::table_size; ++i) {
      unsigned p = PrimesList::table[i];
      mobius[i] = (PrimesList::table[i / p] == p ? 0 : -1 * mobius[i / p]);
    }
  }

  int GetTable(uint64_t n) const {
    assert(n <= PrimesList::table_size);
    unsigned primes_count = 0;
    for (; n > 1;) {
      unsigned p = PrimesList::table[n];
      n /= p;
      if (PrimesList::table[n] == p) return 0;
      ++primes_count;
    }
    return (primes_count & 1) ? -1 : 1;
  }

  int Get(uint64_t n) const {
    if (n < mobius.size()) return mobius[n];
    if (n <= PrimesList::table_size) return GetTable(n);
    unsigned primes_count = 0;
    for (uint64_t p : PrimesList::primes) {
      if (n < p * p) break;
      if ((n % p) == 0) {
        n /= p;
        if ((n % p) == 0) return 0;
      }
    }
    assert(PrimesList::squared_table_size > n);
    if (n != 1) ++primes_count;
    return (primes_count & 1) ? -1 : 1;
  }

  int operator()(uint64_t n) const { return Get(n); }
};
}  // namespace ple
}  // namespace factorization
