// https://www.hackerrank.com/challenges/demidenko-computer-virus

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_computer_virus() {
  ModularFactorial<TModular> f;
  unsigned T, K;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N;
    cin >> K >> N;
    TModular s = 0, nf = 1, p2 = 1, two = 2;
    for (unsigned i = 0; i <= K; ++i) {
      s += p2 * f.BinomialCoefficient(K, i) * nf * f.GetI(i);
      nf *= TModular(N - i);
      p2 *= two;
    }
    cout << s << endl;
  }
  return 0;
}
