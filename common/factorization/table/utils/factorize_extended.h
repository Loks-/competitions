#include "common/base.h"
#include "common/factorization/base.h"
#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/factorize.h"

constexpr TFactorization FactorizeExtended(const PrimesTable& pt, uint64_t n,
                                           bool skip_assert = false) {
  const auto& table = pt.GetTable();
  if (n < table.size()) return Factorize(pt, n);
  TFactorization output;
  for (uint64_t p : pt.GetPrimes()) {
    if (n < p * p) break;
    if ((n % p) == 0) {
      n /= p;
      unsigned cnt = 1;
      for (; (n % p) == 0; ++cnt) n /= p;
      output.push_back({p, cnt});
    }
  }
  if (!skip_assert) assert(pt.GetSquaredTableSize() > n);
  if (n != 1) output.push_back({n, 1});
  return output;
}
