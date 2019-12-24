#include "common/factorization/table/primes.h"

inline bool IsPrime(const PrimesTable& pt, uint64_t n) {
  assert(n <= pt.GetSquaredTableSize());
  if (n <= pt.GetTableSize()) return pt.IsPrime(n);
  const auto& primes = pt.GetPrimes();
  const auto& squared_primes = pt.GetSquaredPrimes();
  for (size_t i = 0; squared_primes[i] <= n; ++i) {
    if ((n % primes[i]) == 0) return false;
  }
  return true;
}
