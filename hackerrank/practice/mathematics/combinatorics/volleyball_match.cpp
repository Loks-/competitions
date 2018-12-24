// https://www.hackerrank.com/challenges/volleyball-match

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_volleyball_match() {
  TFactorial f;
  unsigned n, m;
  cin >> n >> m;
  if (n < m) swap(n, m);
  if ((n < 25) || (n < m + 2) || ((n > 25) && (n != m + 2)))
    cout << 0 << endl;
  else if (n == 25)
    cout << f.BinomialCoefficient(n + m - 1, m) << endl;
  else
    cout << f.BinomialCoefficient(48, 24) * TModular(2).PowU(n - 26) << endl;
  return 0;
}
