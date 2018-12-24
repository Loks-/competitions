// https://www.hackerrank.com/challenges/alien-flowers

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_alien_flowers() {
  TFactorial f;
  unsigned A, B, C, D;
  cin >> A >> B >> C >> D;
  TModular res = 0;
  if (B == D + 1) {
    res = f.BinomialCoefficient(D + A, D) * f.BinomialCoefficient(D + C, D);
  } else if (D == B + 1) {
    res = f.BinomialCoefficient(B + A, B) * f.BinomialCoefficient(B + C, B);
  } else if (B == D) {
    if (D > 0)
      res = f.BinomialCoefficient(D + A, D) *
                f.BinomialCoefficient(D + C - 1, D - 1) +
            f.BinomialCoefficient(D + A - 1, D - 1) *
                f.BinomialCoefficient(D + C, D);
    else if (A + C == 0)
      res = 2;
    else if (A * C == 0)
      res = 1;
  }
  cout << res << endl;
  return 0;
}
