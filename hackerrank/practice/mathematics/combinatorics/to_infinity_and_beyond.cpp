// https://www.hackerrank.com/challenges/to-infinity-and-beyond

#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TFactorial = modular::mstatic::Factorial<TModularD>;

int main_to_infinity_and_beyond() {
  TFactorial f;
  unsigned T;
  cin >> T;
  uint64_t x, y, z;
  for (unsigned it = 0; it < T; ++it) {
    cin >> x >> y >> z;
    unsigned mk = unsigned(min(x, y));
    TModularD s(0), v = f.GetI(mk);
    for (unsigned l = 1; l <= mk; ++l) v *= TModularD(x + y + 1 - l);
    TModularD a(1), b(1), c(1);
    v -= a * b;
    s += v;
    for (unsigned l = 1; l < mk; ++l) {
      a *= TModularD(x + 1 - l);
      b *= TModularD(y + 1 - l);
      c *= TModularD(z - l);
      v -= a * b * f.GetI(l) * f.GetI(l);
      s += v * c * f.GetI(l);
    }
    cout << s << endl;
  }
  return 0;
}
