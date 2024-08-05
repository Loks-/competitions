#include "common/base.h"
#include "common/factorization/table/primes.h"

constexpr int Mobius(const PrimesTable& pt, uint64_t n) {
  const auto& table = pt.GetTable();
  assert(n <= table.size());
  unsigned primes_count = 0;
  for (; n > 1;) {
    unsigned p = table[n];
    n /= p;
    if (table[n] == p) return 0;
    ++primes_count;
  }
  return (primes_count & 1) ? -1 : 1;
}
