#include "common/base.h"
#include "common/factorization/base.h"
#include "common/factorization/table/primes.h"

inline TFactorization Factorize(const PrimesTable& pt, uint64_t n) {
  const auto& table = pt.GetTable();
  assert(n <= table.size());
  TFactorization output;
  for (; n > 1;) {
    unsigned p = table[n], cnt = 1;
    for (n /= p; table[n] == p; ++cnt) n /= p;
    output.push_back({uint64_t(p), cnt});
  }
  return output;
}
