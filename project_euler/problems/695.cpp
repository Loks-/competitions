#include "common/calculus/ext_polynomial/io.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_integration_ext.h"
#include "common/calculus/ext_polynomial/mv_make.h"
#include "common/stl/base.h"
#include <cmath>
#include <iomanip>

using namespace calculus::ext_polynomial;

using DF2 = DMVFunction<2>;

int main_695() {
  auto zero = DF2(), one = DF2(1.), x = DMakeXi<2>(0), y = DMakeXi<2>(1);
  double mblock = 1. / 4, phi = (sqrt(5.) - 1) / 2;

  double p1 = 1. / 3;
  auto f1_2 = x * y * 2.;
  auto f1_1 = IntegrationAB(f1_2, 1, one - x, 1.);
  auto f1 = IntegrationAB(f1_1, 0, 0., 1.);

  double p2 = 2. / 3;
  auto fx_2 = (one - x);
  auto fx1_1 = IntegrationAB(fx_2, 1, 0., x);
  auto fx2_1 = IntegrationAB(fx_2, 1, (one - x) / x, 1.);
  auto fx3_1 = IntegrationAB(fx_2, 1, 0., (one - x) / x);
  auto fx4_1 = IntegrationAB(fx_2, 1, x, 1.);
  auto fx1 = IntegrationAB(fx1_1, 0, 0., phi);
  auto fx2 = IntegrationAB(fx2_1, 0, 0.5, phi);
  auto fx34 = IntegrationAB(fx3_1 + fx4_1, 0, phi, 1.0);
  auto fx = fx1 + fx2 + fx34;
  auto fy = fx;
  auto fz_2 = x * y;
  auto fz1_1 = IntegrationAB(fz_2, 0, phi, 1.0);
  auto fz1 = IntegrationAB(fz1_1, 1, 0., 0.5);
  auto fz2_1 = IntegrationAB(fz_2, 0, phi, (one - y) / y);
  auto fz2 = IntegrationAB(fz2_1, 1, 0.5, phi);
  auto fz3_1 = IntegrationAB(fz_2, 1, 0., (one - x) / x);
  auto fz3 = IntegrationAB(fz3_1, 0, phi, 1.);
  auto fz = 2.0 * (fz1 + fz2 - fz3);
  auto f2 = fx + fy + fz;

  auto f = mblock * (p1 * f1 + p2 * f2);
  cout << setprecision(10) << fixed << f.ToValue() << endl;
  return 0;
}
