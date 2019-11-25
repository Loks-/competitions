// https://www.hackerrank.com/challenges/eugene-and-big-number

#include "common/modular/proxy/proxy.h"
#include "common/stl/base.h"

using TModularProxy = modular::proxy::Proxy<false, false, uint64_t>;

int main_eugene_and_big_number() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t A, N, M, m = 9, l = 1;
    cin >> A >> N >> M;
    for (; m < A; ++l) m = 10 * m + 9;
    TModularProxy mp(M * m);
    cout << mp.MultSafe(mp.Sub(mp.PowUSafe(10, N * l), 1), A) / m << endl;
  }
  return 0;
}
