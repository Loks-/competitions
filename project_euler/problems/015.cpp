#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

// Time: O(T + N + M)
// Memory: O(N + M)
int main_015() {
  modular::mstatic::Factorial<TModularD> f;
  unsigned T, N, M;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> M;
    cout << f.BinomialCoefficient(N + M, N) << endl;
  }
  return 0;
}
