#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/functions.h"
#include "common/calculus/ext_polynomial/multivariable/io.h"
#include "common/calculus/ext_polynomial/multivariable/operators.h"
#include "common/calculus/ext_polynomial/multivariable/operators/integration.h"
#include "common/stl/base.h"

using namespace calculus::ext_polynomial::multivariable;
using namespace calculus::ext_polynomial::multivariable::operators;

int main_695() {
  auto zero = DFunction<4>(), one = DFunction<4>(1.), x0 = DMakeXi<4>(0),
       y0 = DMakeXi<4>(1), x1 = DMakeXi<4>(2), y1 = DMakeXi<4>(3);
  auto fc = one;
  // 0 <= x0 <= 1, 0 <= y0 <= 1
  // Segment 1: 0 <= x1 <= x0, y0 <= y1 <= 1
  auto s1p = fc * (1. - x0) * (1. - y1);
  // Segment 2: x0 <= x1 <= 1, y0 <= y1 <= 1
  auto s2p = fc * (1. - x1) * (1. - y1);
  //   Substitute x1 = x0 + xs, y1 = y0 + ys
  //   After S: 0 <= xs <= 1 - x0, 0 <= ys <= 1 - y0
  auto s2ps = Substitution(Substitution(s2p, 2, x0 + x1), 3, y0 + y1);
  cout << s2ps << endl;
  //   Region 2.1: (x1 - x0) * (y1 - y0) <= x0 * y0
  //     After S: xs * ys <= x0 * y0
  //     Region 2.1.1: 1 <= x0 + y0
  auto f4_211 = s2ps;
  auto f3_211 = Integration(f4_211, 3, zero, one - y0);
  auto f2_211 = Integration(f3_211, 2, zero, one - x0);
  cout << f2_211 << endl;
  //     Region 2.1.2: x0 + y0 <= 1
  auto f4_212 = s2ps;
  auto f3_212c = Integration(f4_212, 3, x0 * y0 / x1, one - y0);
  cout << f3_212c << endl;
  // Segment 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  auto s3p = fc * (1. - x1) * (1. - y0);
  // Segment 4: x0 <= x1 <= 1, y0 - 1 <= y1 <= 0
  auto s4p = fc * (1. - x1) * (1. + y1 - y0);
  return 0;
}
