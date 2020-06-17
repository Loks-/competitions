#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

// Time: O(T + N + M)
// Memory: O(N + M)
int main_015() {
  unsigned T, N, M;
  modular::mstatic::Factorial<TModularD> f;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> M;
    cout << f.BinomialCoefficient(N + M, N) << endl;
  }
  return 0;
}
