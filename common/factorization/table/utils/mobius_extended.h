#include "common/base.h"
#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/mobius.h"

inline int MobiusExtended(const PrimesTable& pt, uint64_t n) {
  if (n <= pt.GetTableSize()) return Mobius(pt, n);
  unsigned primes_count = 0;
  for (uint64_t p : pt.GetPrimes()) {
    if (n < p * p) break;
    if ((n % p) == 0) {
      n /= p;
      if ((n % p) == 0) return 0;
    }
  }
  assert(pt.GetSquaredTableSize() > n);
  if (n != 1) ++primes_count;
  return (primes_count & 1) ? -1 : 1;
}
