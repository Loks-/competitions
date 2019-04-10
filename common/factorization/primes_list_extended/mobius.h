#pragma once

#include "common/factorization/primes_list.h"
#include <vector>

template <class TPrimesList>
class PLEMobius : public TPrimesList {
 protected:
  std::vector<int> mobius;

 public:
  PLEMobius(uint64_t size) : TPrimesList(size) {}

  void PrecalcMobius() {
    mobius.resize(TPrimesList::table_size + 1);
    mobius[0] = 0;
    mobius[1] = 1;
    for (unsigned i = 2; i <= TPrimesList::table_size; ++i) {
      unsigned p = TPrimesList::table[i];
      mobius[i] = (TPrimesList::table[i / p] == p ? 0 : -1 * mobius[i / p]);
    }
  }

  int MobiusTable(uint64_t n) const {
    assert(n <= TPrimesList::table_size);
    unsigned primes_count = 0;
    for (; n > 1;) {
      unsigned p = TPrimesList::table[n];
      n /= p;
      if (TPrimesList::table[n] == p) return 0;
      ++primes_count;
    }
    return (primes_count & 1) ? -1 : 1;
  }

  int Mobius(uint64_t n) const {
    if (n < mobius.size()) return mobius[n];
    if (n <= TPrimesList::table_size) return MobiusTable(n);
    unsigned primes_count = 0;
    for (uint64_t p : TPrimesList::primes) {
      if (n < p * p) break;
      if ((n % p) == 0) {
        n /= p;
        if ((n % p) == 0) return 0;
      }
    }
    assert(TPrimesList::squared_table_size > n);
    if (n != 1) ++primes_count;
    return (primes_count & 1) ? -1 : 1;
  }
};

using TPLEMobius = PLEMobius<PrimesList>;
