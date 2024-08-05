#include "common/base.h"
#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/eulers_totient.h"

constexpr uint64_t EulersTotientExtended(const PrimesTable& pt, uint64_t n) {
  const auto& table = pt.GetTable();
  if (n <= table.size()) return EulersTotient(pt, n);
  uint64_t output = n;
  for (uint64_t p : pt.GetPrimes()) {
    if (n < p * p) break;
    if ((n % p) == 0) {
      output -= output / p;
      for (n /= p; (n % p) == 0;) n /= p;
    }
  }
  assert(n < pt.GetSquaredTableSize());
  if (n != 1) output -= (output / n);
  return output;
}
