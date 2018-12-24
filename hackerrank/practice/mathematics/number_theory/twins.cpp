// https://www.hackerrank.com/challenges/twins

#include "common/factorization/primality_test.h"
#include "common/stl/base.h"

int main_twins() {
  MillerRabinPrimalityTest pt;
  unsigned n, m, s = 0;
  cin >> n >> m;
  for (unsigned i = n | 1; i + 2 <= m; i += 2) {
    if (pt.IsPrime(i) && pt.IsPrime(i + 2)) s += 1;
  }
  cout << s << endl;
  return 0;
}
