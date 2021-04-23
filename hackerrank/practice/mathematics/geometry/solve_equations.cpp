// https://www.hackerrank.com/challenges/solve-equations

#include "common/modular/arithmetic.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

int main_solve_equations() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int64_t a, b, c;
    cin >> a >> b >> c;
    int64_t d = GCD(a, b);
    a /= d;
    b /= d;
    c /= d;
    int64_t x0 = modular::TArithmetic_C32U::DivSafe_Composite(c, a, b);
    if (x0 == 0) x0 += b;
    int64_t x1 = a * c / (a * a + b * b);
    if (x1 > x0) {
      x1 -= (x1 - x0) % b;
      int64_t y1 = (c - a * x1) / b, x2 = 1 + b, y2 = (c - a * x2) / b;
      if (x2 * x2 + y2 * y2 < x1 * x1 + y1 * y1) {
        x1 = x2;
        y1 = y2;
      }
      cout << x1 << " " << y1 << endl;
    } else {
      cout << x0 << " " << (c - a * x0) / b << endl;
    }
  }
  return 0;
}
