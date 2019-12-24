#include "common/factorization/primes_list_extended/mobius.h"
#include "common/numeric/long/pow.h"
#include "common/numeric/long/unsigned_io.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(N^(1/4) * log(log(N)))
// Memory: O(N^(1/4))
int main_641() {
  LongUnsigned N = numeric::nlong::PowU(10u, 36);
  unsigned M = PowU(10, 9), l = PowU(10, 6);
  auto plm = factorization::ple::Mobius(M, false);
  cout << "Mobius pregenerated" << endl;
  uint64_t s = 0;
  for (unsigned b = 1; numeric::nlong::PowU(b, 4) <= N; ++b) {
    if (plm.Get(b) == 1) {
      LongUnsigned t = N;
      for (unsigned j = 0; j < 4; ++j) t /= b;
      for (; t < numeric::nlong::PowU(l, 6);) --l;
      s += l;
    }
  }
  cout << s << endl;
  return 0;
}
