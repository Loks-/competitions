#include "common/factorization/mertens.h"
#include "common/factorization/table/mobius.h"
#include "common/factorization/table/utils/square_free_count.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

// Time: O(N^(1/6) * log(log(N)))
// Memory: O(N^(1/6))
int main_641() {
  uint64_t NSqrt = PowU<uint64_t>(10, 18);
  uint64_t s = 0, l = UCbrt(NSqrt);
  factorization::table::Mobius mobius(USqrt(USqrt(NSqrt)));
  factorization::Mertens mertens(l);
  for (uint64_t a = 1; a <= l; ++a) {
    uint64_t x = USqrt(NSqrt / (a * a * a));
    s += mertens(x) + SquareFreeCount(mobius, x);
  }
  cout << s / 2 << endl;
  return 0;
}
