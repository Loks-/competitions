#include "common/factorization/table/mobius.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

// Time: O(N^(1/4) * log(log(N)))
// Memory: O(N^(1/4))
int main_641__slow() {
  uint64_t NSqrt = PowU<uint64_t>(10, 18), M = USqrt(NSqrt);
  uint64_t s = 0, l = UCbrt(NSqrt);
  factorization::table::Mobius mobius(M);
  cout << "Mobius pregenerated" << endl;
  for (uint64_t b = 1; b * b <= NSqrt; ++b) {
    if (mobius(b) == 1) {
      uint64_t t = NSqrt / (b * b);
      for (; t < l * l * l;) --l;
      s += l;
    }
  }
  cout << s << endl;
  return 0;
}
