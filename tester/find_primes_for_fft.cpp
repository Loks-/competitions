#include "common/base.h"
#include "common/factorization/primality_test.h"

#include <iostream>

void FindPrimesForModularFFT(unsigned count) {
  uint64_t max_value = (1u << 31);
  unsigned found = 0;
  for (unsigned k = 30; (found < count) && (k > 0); --k) {
    uint64_t p2k = (1u << k);
    for (uint64_t n = p2k + 1; n < max_value; n += p2k) {
      if (IsPrime(n)) {
        std::cout << k << "\t" << n << std::endl;
        if (++found == count) break;
      }
    }
  }
}
