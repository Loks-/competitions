#include "common/factorization/table/mobius.h"
#include "common/factorization/utils/cube_free_count.h"
#include "common/factorization/utils/square_free.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

// Time: O(N^(1/3) * log(log(N)))
// Memory: O(N^(1/3))
int main_634() {
  uint64_t N = 9 * PowU<uint64_t>(10, 18), M = UCbrt(N);
  factorization::table::Mobius mobius(M);
  uint64_t SqrtN = USqrt(N), s = 0;
  for (uint64_t b = 2; b * b * b <= N / 4; ++b) {
    if (mobius(b)) s += USqrt(N / (b * b * b)) - 1;
  }
  for (uint64_t b = 2; b * b * b <= SqrtN; ++b) {
    if (mobius.IsPrime(b)) s -= 1;
    s += CubeFreeCount(SqrtN / (b * b * b), mobius.GetPrimes());
  }
  cout << s << endl;
  return 0;
}
