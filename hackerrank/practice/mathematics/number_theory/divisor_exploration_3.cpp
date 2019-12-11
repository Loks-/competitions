// https://www.hackerrank.com/challenges/divisor-exploration-3

#include "common/factorization/primes_generator.h"
#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TFactorial = modular::mstatic::Factorial<TModularD, true>;

int main_divisor_exploration_3() {
  TFactorial f;
  vector<uint64_t> primes = GeneratePrimes(100000);
  unsigned T, m, a, d;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    TModularD r = 1, one = 1;
    cin >> m >> a >> d;
    for (unsigned i = 0; i < m; ++i) {
      TModularD s = 0;
      s += f.BinomialCoefficient(a + i + d, d - 1);
      TModularD p = primes[i], p1 = p - one, pp = p1;
      for (unsigned j = 1; j <= a + i + 1; ++j) {
        s += f.BinomialCoefficient(a + i + d - j, d - 1) * pp;
        pp *= p;
      }
      r *= s;
    }
    cout << r << endl;
  }
  return 0;
}
