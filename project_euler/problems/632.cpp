#include "common/factorization/table/mobius.h"
#include "common/factorization/table/utils/factorize.h"
#include "common/factorization/table/utils/square_free_count.h"
#include "common/modular_io.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

int main_632() {
  uint64_t N = PowU<uint64_t>(10, 16), NSqrt = USqrt(N);
  factorization::table::Mobius mobius(NSqrt);
  vector<uint64_t> vsk;
  for (uint64_t a = 1; a <= NSqrt; ++a) {
    size_t k = Factorize(mobius, a).size();
    if (vsk.size() <= k) vsk.resize(k + 1);
    vsk[k] += SquareFreeCount(mobius, N / (a * a));
  }
  TModularD m = 1;
  for (uint64_t sk : vsk) m *= TModularD(sk);
  cout << m << endl;
  return 0;
}
