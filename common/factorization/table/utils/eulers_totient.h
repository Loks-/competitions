#include "common/base.h"
#include "common/factorization/table/primes.h"

constexpr uint64_t EulersTotient(const PrimesTable& pt, uint64_t n) {
  const auto& table = pt.GetTable();
  assert(n <= table.size());
  uint64_t output = n;
  for (; n > 1;) {
    unsigned p = table[n];
    output -= output / p;
    for (n /= p; table[n] == p;) n /= p;
  }
  return output;
}
