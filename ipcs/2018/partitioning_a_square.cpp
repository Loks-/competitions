#include "common/factorization/utils/factorization_base.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

int main_partitioning_a_square() {
  unsigned T, N;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> N;
    TFactorization f = FactorizeBase(N);
    unsigned l = 1;
    for (auto p : f) l *= PowU(unsigned(p.prime), (p.power + 1) / 2);
    cout << l << endl;
    unsigned k = (l * l) / N;
    for (unsigned i = 0; i < l; ++i) {
      for (unsigned j = 0; j < l; ++j) cout << char('a' + (i * l + j) / k);
      cout << endl;
    }
  }
  return 0;
}
