#include "common/calculus/ext_polynomial/multivariable/function.h"
#include "common/calculus/ext_polynomial/multivariable/functions.h"
#include "common/calculus/ext_polynomial/multivariable/io.h"
#include "common/calculus/ext_polynomial/multivariable/operators.h"
#include "common/stl/base.h"

using namespace calculus::ext_polynomial::multivariable;

int main_695() {
  auto x0 = DMakeXi<4>(0), y0 = DMakeXi<4>(1), x1 = DMakeXi<4>(2),
       y1 = DMakeXi<4>(3);
  auto s1p = (1. - x0) * (1. - y1), s2p = (1. - x1) * (1. - y1),
       s3p = (1. - x1) * (1. - y0), s4p = (1. - x1) * (1. + y1 - y0);
  cout << s1p << endl;
  //  auto fp1 = (1 - DMakeXi<double, 4>(0))
  return 0;
}
