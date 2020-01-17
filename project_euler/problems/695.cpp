#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/functions.h"
#include "common/calculus/ext_polynomial/multivariable/io.h"
#include "common/calculus/ext_polynomial/multivariable/operators.h"
#include "common/calculus/ext_polynomial/multivariable/operators/integration.h"
#include "common/stl/base.h"

using namespace calculus::ext_polynomial::multivariable;
using namespace calculus::ext_polynomial::multivariable::operators;

int main_695() {
  auto x0 = DMakeXi<4>(0), y0 = DMakeXi<4>(1), x1 = DMakeXi<4>(2),
       y1 = DMakeXi<4>(3);
  auto fc = DFunction<4>(1.);
  // 0 <= x0 <= 1, 0 <= y0 <= 1
  // Segment 1: 0 <= x1 <= x0, y0 <= y1 <= 1
  auto s1p = fc * (1. - x0) * (1. - y1);
  // Segment 2: x0 <= x1 <= 1, y0 <= y1 <= 1
  auto s2p = fc * (1. - x1) * (1. - y1);
  cout << s2p << endl;
  // Region 2.1: (x1 - x0) * (y1 - y0) <= x0 * y0
  // Region 2.1.1: 1 <= x0 + y0
  auto f00_211 = Integration(s2p, 3);
  cout << f00_211 << endl;
  // Segment 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  auto s3p = fc * (1. - x1) * (1. - y0);
  // Segment 4: x0 <= x1 <= 1, y0 - 1 <= y1 <= 0
  auto s4p = fc * (1. - x1) * (1. + y1 - y0);
  return 0;
}
