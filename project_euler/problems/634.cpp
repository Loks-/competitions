#include "common/factorization/primes_list.h"
#include "common/factorization/utils/cube_free_count.h"
#include "common/factorization/utils/square_free.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

#include <functional>

int main_634() {
  uint64_t N = 2000000, M = 9000000000000000000ull;
  factorization::PrimesList primes_list(N);
  uint64_t SqrtM = USqrt(M), s = 0;
  for (uint64_t b = 2; b * b * b <= M / 4; ++b) {
    if (SquareFree(primes_list.FactorizeTable(b))) {
      s += USqrt(M / (b * b * b)) - 1;
    }
  }
  for (uint64_t b = 2; b * b * b <= SqrtM; ++b) {
    if (primes_list.IsPrime(b)) s -= 1;
    s += CubeFreeCount(SqrtM / (b * b * b), primes_list.GetPrimes());
  }
  cout << s << endl;
  return 0;
}
