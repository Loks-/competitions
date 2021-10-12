// https://www.hackerrank.com/challenges/super-humble-matrix

#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TFactorial = modular::mstatic::Factorial<TModularD, false>;

int main_super_humble_matrix() {
  TFactorial f;
  unsigned n, m;
  cin >> n >> m;
  if (n < m) swap(n, m);
  TModularD r = 1;
  f.Adjust(m);
  for (unsigned i = 1; i < m; ++i) r *= f(i) * f(i);
  r *= f(m).PowU(n - m + 1);
  cout << r << endl;
  return 0;
}
