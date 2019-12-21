// https://www.hackerrank.com/challenges/fun-with-1010

#include "common/modular/static/binomial_coefficient_prime.h"
#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<2000003>;

int main_fun_with_1010() {
  TModular h = TModular(2).Inverse();
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N, M;
    cin >> N >> M;
    N -= 1;
    TModular n(N), p2n1 = TModular(2).PowU(N - 1), p2n = p2n1 * 2;
    TModular r =
        (n + 2) * p2n1 * (p2n * (p2n - 3) + 3) -
        n * modular::mstatic::BinomialCoefficientPrime<TModular>(2 * N, N) * 3 *
            h * (p2n1 - 1) -
        (n + 1);
    r += (p2n - 1).PowU(3) * (M - N);
    cout << r << endl;
  }
  return 0;
}
