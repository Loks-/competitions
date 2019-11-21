// https://www.hackerrank.com/challenges/div-and-span

#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TFactorial = ModularFactorial<TModularD>;

int main_div_and_span() {
  TFactorial f;
  unsigned T, X, Y;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> X >> Y;
    cout << f(2 * X + 1) * f.GetI(X + 1) * f(2 * X + 2 * Y) *
                f.GetI(2 * X + Y + 1)
         << endl;
  }
  return 0;
}
