#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/factorize.h"
#include "common/factorization/utils/divisors.h"
#include "common/stl/base.h"

int main_matrygons() {
  unsigned N = 1000001, T, n;
  PrimesTable pt(N);
  vector<unsigned> v(N, 0);
  for (unsigned i = 2; i < N; ++i) {
    auto fi = Factorize(pt, i);
    auto vd = GetDivisors(fi);
    v[i] = 1;
    for (auto d : vd) {
      if ((d > 1) && (d < i)) v[i] = max(v[i], 1 + v[i / d - 1]);
    }
  }
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> n;
    unsigned k = 1;
    if ((n % 2) == 0) {
      auto fn = Factorize(pt, n);
      auto vd = GetDivisors(fn);
      for (auto d : vd) {
        if ((d > 2) && (d < n)) k = max(k, 1 + v[n / d - 1]);
      }
    } else {
      k = v[n];
    }
    cout << "Case #" << it << ": " << k << endl;
  }
  return 0;
}
