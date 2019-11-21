// https://www.hackerrank.com/challenges/demidenko-computer-virus

#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

int main_computer_virus() {
  ModularFactorial<TModularD> f;
  unsigned T, K;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N;
    cin >> K >> N;
    TModularD s = 0, nf = 1, p2 = 1, two = 2;
    for (unsigned i = 0; i <= K; ++i) {
      s += p2 * f.BinomialCoefficient(K, i) * nf * f.GetI(i);
      nf *= TModularD(N - i);
      p2 *= two;
    }
    cout << s << endl;
  }
  return 0;
}
