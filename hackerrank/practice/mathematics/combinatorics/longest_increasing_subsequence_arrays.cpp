// https://www.hackerrank.com/challenges/longest-increasing-subsequence-arrays

#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TFactorial = modular::mstatic::Factorial<TModularD>;

int main_longest_increasing_subsequence_arrays() {
  TFactorial f;
  unsigned T, n, m;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> m >> n;
    TModularD s = 0, k = TModularD(n - 1) / TModularD(n), p = 1;
    for (unsigned l = n; l <= m; ++l, p *= k)
      s += f.BinomialCoefficient(l - 1, n - 1) * p;
    cout << s * TModularD(n).PowU(m - n) << endl;
  }
  return 0;
}
