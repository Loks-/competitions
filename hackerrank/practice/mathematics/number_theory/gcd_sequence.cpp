// https://www.hackerrank.com/challenges/gcd-sequence

#include "common/factorization/table/mobius.h"
#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

int main_gcd_sequence() {
  factorization::table::Mobius mobius(100000);
  modular::mstatic::Factorial<TModularD> f;

  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> K;
    TModularD r = 0, one = 1;
    for (unsigned i = 1; i <= N; ++i) {
      int m = mobius(i);
      if (m == 0) continue;
      r += ((m == 1) ? one : -one) * f.BinomialCoefficient(K + N / i - 1, K);
    }
    cout << r << endl;
  }
  return 0;
}
