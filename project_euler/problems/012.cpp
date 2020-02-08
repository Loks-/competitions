#include "common/factorization/table/divisors_count.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(T * M + M * log(M))
// Memory: O(M)
int main_012() {
  unsigned T, N, M = PowU(10u, 5);
  factorization::table::DivisorsCount dc(M);
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    uint64_t s = 0;
    unsigned dcj = 1;
    for (unsigned j = 1;; ++j) {
      s += j;
      unsigned dcj1 = dc((j & 1) ? (j + 1) / 2 : j + 1);
      if (dcj * dcj1 > N) {
        cout << s << endl;
        break;
      }
      dcj = dcj1;
    }
  }
  return 0;
}
