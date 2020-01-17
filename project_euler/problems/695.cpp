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
  // auto fc = x0 * y0;
  // 0 <= x0 <= 1, 0 <= y0 <= 1
  // Segment 1: 0 <= x1 <= x0, y0 <= y1 <= 1
  auto s1p = fc * (one - x0) * (one - y1);
  // Segment 2: x0 <= x1 <= 1, y0 <= y1 <= 1
  auto s2p = fc * (one - x1) * (one - y1);
  //   Substitute1 x1 = x0 + x1s, y1 = y0 + y1s
  //   After S1: 0 <= x1s <= 1 - x0, 0 <= y1s <= 1 - y0
  auto s2ps1 = Substitution(Substitution(s2p, 2, x0 + x1), 3, y0 + y1);
  //   Substitute2 x0 = 1 - x0s, y0 = 1 - y0s
  //   After S2: 0 <= x1s <= x0s, 0 <= y1s <= y0s
  auto s2ps2 = Substitution(Substitution(s2ps1, 0, one - x0), 1, one - y0);
  cout << s2ps2 << endl;
  //   Region 2.1: (x1 - x0) * (y1 - y0) <= x0 * y0
  //     After S1: x1s * y1s <= x0 * y0
  //     After S2: x1s * y1s <= (1 - x0s) * (1 - y0s)
  //     Region 2.1.1: 1 <= x0 + y0
  //       After S2: x0s + y0s <= 1
  auto f4_211 = s2ps2;
  auto f3_211 = Integration(f4_211, 3, zero, y0);
  auto f2_211 = Integration(f3_211, 2, zero, x0);
  cout << f2_211 << endl;
  //     Region 2.1.2: x0 + y0 <= 1
  //       After S2: 1 <= x0s + y0s
  auto f4_212 = s2ps2;
  auto f3_212c = Integration(f4_212, 3, (one - x0) * (one - y0) / x1, y0);
  cout << f3_212c << endl;
  // a = (one - x0) * (one - y0) / y0, b = x0
  auto f3_212c_ii = Integration(f3_212c, 2);
  cout << f3_212c_ii << endl;
  auto f2_212c_b = SubstitutionIndex(f3_212c_ii, 2, 0);
  cout << f2_212c_b << endl;
  // cout << Project(f3_212c_ii, 0, 1.) << endl;
  // Segment 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  auto s3p = fc * (1. - x1) * (1. - y0);
  // Segment 4: x0 <= x1 <= 1, y0 - 1 <= y1 <= 0
  auto s4p = fc * (1. - x1) * (1. + y1 - y0);
  return 0;
}
