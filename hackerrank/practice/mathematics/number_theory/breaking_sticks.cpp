// https://www.hackerrank.com/challenges/breaking-sticks

#include "common/factorization/factorization.h"
#include "common/stl/base.h"

int main_breaking_sticks() {
  unsigned n;
  cin >> n;
  uint64_t sum = n, a;
  for (unsigned i = 0; i < n; ++i) {
    cin >> a;
    TFactorization vf = Factorize(a);
    reverse(vf.begin(), vf.end());
    uint64_t t = 1;
    for (auto p : vf) {
      for (unsigned j = 0; j < p.power; ++j) sum += (t *= p.prime);
    }
  }
  cout << sum << endl;
  return 0;
}
