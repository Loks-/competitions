#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

int main_intranets() {
  unsigned T, M, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> M >> K;
    modular::mstatic::Factorial<TModularD> f;
    f.Adjust(M);
    vector<TModularD> vx(M / 2 + 1, 0);
    TModularD m = 1;
    for (unsigned i = 1; i <= M / 2; ++i) {
      m *= f.BinomialCoefficient(M, 2) - f.BinomialCoefficient(M - 2 * i, 2);
      m *= 2;
      vx[i] = f.GetI(M - 2 * i) / m;
    }
    TModularD r = 0, m1 = 1;
    for (unsigned i = K; i <= M / 2; ++i) {
      r += m1 * vx[i] * f.BinomialCoefficient(i, K);
      m1 = -m1;
    }
    cout << "Case #" << it << ": " << r * f(M) << endl;
  }
  return 0;
}
