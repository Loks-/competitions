// https://www.hackerrank.com/challenges/sherlock-and-permutations

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular, true>;

int main_sherlock_and_permutations() {
  TFactorial f;
  unsigned T, N, M;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> M;
    cout << f.BinomialCoefficient(N + M - 1, N) << endl;
  }
  return 0;
}
