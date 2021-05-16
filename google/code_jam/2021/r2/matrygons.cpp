#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/factorize.h"
#include "common/factorization/utils/divisors.h"
#include "common/stl/base.h"

int main_matrygons() {
  unsigned N = 1000001, T, n;
  PrimesTable pt(N);
  vector<unsigned> v2{0, 0, 1}, v3(3, 0);
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> n;
    for (; v3.size() <= n;) {
      unsigned i = v3.size();
      unsigned k3 = 1;
      for (auto d : GetDivisors(Factorize(pt, i))) {
        if (d > 2) k3 = max(k3, 1 + v2[i / d - 1]);
      }
      v3.push_back(k3);
      v2.push_back(((i % 2) == 0) ? max(k3, 1 + v2[i / 2 - 1]) : k3);
    }
    cout << "Case #" << it << ": " << v3[n] << endl;
  }
  return 0;
}
