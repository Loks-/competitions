#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/io.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_integration_ext.h"
#include "common/calculus/ext_polynomial/mv_make.h"
#include "common/calculus/ext_polynomial/mv_substitution_function.h"
#include "common/stl/base.h"
#include <cmath>

using namespace calculus::ext_polynomial;

using DF4 = DMVFunction<4>;

int main_695() {
  double phi = (sqrt(5.) - 1.) / 2;
  auto zero = DF4(), one = DF4(1.), x0 = DMakeXi<4>(0), y0 = DMakeXi<4>(1),
       x1 = DMakeXi<4>(2), y1 = DMakeXi<4>(3);
  auto fc = one;
  // auto fc = 24. * one;
  // auto fc = x0 * y0;
  // auto fc = 24. * x0 * y0;
  // auto fc = 8. * x0 * y0 * (1. - x0) * (1. - y0);

  // 0 <= x0 <= 1, 0 <= y0 <= 1
  // Region 1: 0 <= x1 <= x0, y0 <= y1 <= 1
  auto f1_4 = fc * (one - x0) * (one - y1);
  //   xc_1 = (1 - phi) * x0, yc_1 = (2 + phi) * y0
  //   Region 1.1:
  //   0 <= x1 <= xc_1
  //   x1 * y1 <= x0 * y0
  //   (x1 - x0) * (y1 - y0) <= -x0 * y0
  //     2 * y0 <= y1 <= 1
  //     Region 1.1.1: y1 <= yc_1
  //       (x1 - x0) * (y1 - y0) <= -x0 * y0
  //       Substitute1: y1 = y0 + y1s
  //         y1s = y1 - y0
  //         y0 <= y1s <= 1 - y0
  //         (x1 - x0) * y1s <= -x0 * y0
  auto f111_4 = SubstitutionFunction(f1_4, 3, y0 + y1);
  cout << "\tf111_4 = " << f111_4 << endl;
  auto f111_3 = IntegrationAB(f111_4, 2, 0., x0 - x0 * y0 / y1);
  cout << "\tf111_3 = " << f111_3 << endl;
  //       Region 1.1.1.1: 1 <= yc_1
  //         1 <= (2 + phi) * y0
  //         1 - phi <= y0
  auto f1111_2 = IntegrationAB(f111_3, 3, y0, DMakeFXi(1, 1., -1.));
  cout << "\tf1111_2 = " << f1111_2 << endl;
  auto f1111_1 = IntegrationAB(f1111_2, 1, 1 - phi, 0.5);
  cout << "\tf1111_1 = " << f1111_1 << endl;
  //       Region 1.1.1.2: yc_1 < 1
  //         y0 < 1 - phi
  auto f1112_2 = IntegrationAB(f111_3, 3, y0, DMakeFXi(1, 0., 1 + phi));
  cout << "\tf1112_2 = " << f1112_2 << endl;
  auto f1112_1 = IntegrationAB(f1112_2, 1, 0., 1 - phi);
  cout << "\tf1112_1 = " << f1112_1 << endl;
  auto f111_1 = f1111_1 + f1112_1;
  cout << "\tf111_1 = " << f111_1 << endl;
  //     Region 1.1.2: yc_1 < y1
  //       x1 * y1 <= x0 * y0
  //       (2 + phi) * y0 < y1 <= 1
  //       y0 < 1 - phi
  auto f112_3 = IntegrationAB(f1_4, 2, 0., x0 * y0 / y1);
  cout << "\tf112_3 = " << f112_3 << endl;
  auto f112_2 = IntegrationAB(f112_3, 3, DMakeFXi(1, 0, 2 + phi), 1.);
  cout << "\tf112_2 = " << f112_2 << endl;
  auto f112_1 = IntegrationAB(f112_2, 1, 0., 1 - phi);
  cout << "\tf112_1 = " << f112_1 << endl;
  auto f11_1 = f111_1 + f112_1;
  cout << "\tf11_1 = " << f11_1 << endl;
  //   Region 1.2:
  //   xc_1 <= x1 <= x0
  //   x0 * y0 <= x1 * y1
  //   -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //     Region 1.2.1: y1 <= yc_1
  //       x0 * y0 <= x1 * y1
  //       x1 <= 1
  auto f121_3 = IntegrationAB(f1_4, 2, x0 * y0 / y1, x0);
  cout << "\tf121_3 = " << f121_3 << endl;
  //       Region 1.2.1.1: 1 <= yc_1
  //         1 <= (2 + phi) * y0
  //         1 - phi <= y0
  auto f1211_2 = IntegrationAB(f121_3, 3, y0, 1.);
  cout << "\tf1211_2 = " << f1211_2 << endl;
  auto f1211_1 = IntegrationAB(f1211_2, 1, 1 - phi, 1.0);
  cout << "\tf1211_1 = " << f1211_1 << endl;
  //       Region 1.2.1.2: yc_1 < 1
  //         y0 < 1 - phi
  auto f1212_2 = IntegrationAB(f121_3, 3, y0, DMakeFXi(1, 0., 2 + phi));
  cout << "\tf1212_2 = " << f1212_2 << endl;
  auto f1212_1 = IntegrationAB(f1212_2, 1, 0., 1 - phi);
  cout << "\tf1212_1 = " << f1212_1 << endl;
  auto f121_1 = f1211_1 + f1212_1;
  cout << "\tf121_1 = " << f121_1 << endl;
  //     Region 1.2.2: yc_1 < y1
  //       -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //       (2 + phi) * y0 < y1 <= 1
  //       y0 < 1 - phi
  //       Substitute1: y1 = y0 + y1s
  //         y1s = y1 - y0
  //         (1 + phi) * y0 <= y1s <= 1 - y0
  //         -x0 * y0 <= (x1 - x0) * y1s
  auto f122_4 = SubstitutionFunction(f1_4, 3, y0 + y1);
  cout << "\tf122_4 = " << f122_4 << endl;
  auto f122_3 = IntegrationAB(f122_4, 2, x0 - x0 * y0 / y1, x0);
  cout << "\tf122_3 = " << f122_3 << endl;
  auto f122_2 =
      IntegrationAB(f122_3, 3, DMakeFXi(1, 0, 1 + phi), DMakeFXi(1, 1., -1));
  cout << "\tf122_2 = " << f122_2 << endl;
  auto f122_1 = IntegrationAB(f122_2, 1, 0., 1 - phi);
  cout << "\tf122_1 = " << f122_1 << endl;
  auto f12_1 = f121_1 + f122_1;
  cout << "\tf12_1 = " << f12_1 << endl;
  auto f1_1 = f11_1 + f12_1;
  cout << "\tf1_1 = " << f1_1 << endl;
  auto f1_0 = IntegrationAB(f1_1, 0, 0., 1.);
  cout << "\tf1_0 = " << f1_0 << endl;

  // Region 2: x0 <= x1 <= 1, y0 <= y1 <= 1
  //   (x1 - x0) * (y1 - y0) <= x0 * y0
  auto f2_4 = fc * (one - x1) * (one - y1);
  //   Substitute1: x1 = x0 + x1s, y1 = y0 + y1s
  //   After S1:
  //     0 <= x1s <= 1 - x0, 0 <= y1s <= 1 - y0
  //     x1s * y1s <= x0 * y0
  auto f2s_4 =
      SubstitutionFunction(SubstitutionFunction(f2_4, 2, x0 + x1), 3, y0 + y1);
  //   Region 2.1: Ignore x1s * y1s <= x0 * y0
  auto f21_3 = IntegrationAB(f2s_4, 3, 0., one - y0);
  cout << "\tf21_3 = " << f21_3 << endl;
  auto f21_2 = IntegrationAB(f21_3, 2, 0., one - x0);
  cout << "\tf21_2 = " << f21_2 << endl;
  auto f21_1 = IntegrationAB(f21_2, 1, 0., 1.);
  cout << "\tf21_1 = " << f21_1 << endl;
  //    Region 2.2 (s): x0 * y0 < x1s * y1s
  //      x0 + y0 < 1
  auto f22_3 = IntegrationAB(f2s_4, 3, x0 * y0 / x1, one - y0);
  cout << "\tf22_3 = " << f22_3 << endl;
  auto f22_2 =
      IntegrationAB(f22_3, 2, DMakeFXi(0) * DMakeFXi(1) / DMakeFXi(1, 1., -1.),
                    DMakeFXi(0, 1., -1.));
  cout << "\tf22_2 = " << f22_2 << endl;
  auto f22_1 = IntegrationAB(f22_2, 1, 0., DMakeFXi(0, 1., -1.));
  cout << "\tf22_1 = " << f22_1 << endl;
  auto f2_1 = f21_1 - f22_1;
  cout << "\tf2_1 = " << f2_1 << endl;
  auto f2_0 = IntegrationAB(f2_1, 0, 0., 1.);
  cout << "\tf2_0 = " << f2_0 << endl;

  // Region 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  //   Similar to Region 1 under (x0 <-> y0) and (x1 <-> y1) swap
  auto f3_0 = f1_0;
  cout << "\tf3_0 = " << f3_0 << endl;

  // Region 4: x0 <= x1 <= 1, y0 - 1 <= y1 <= 0
  auto f4_4 = fc * (1. - x1) * (1. + y1 - y0);
  //   xc_4 = (1 + phi) * x0, yc_4 = -phi * y0
  //   Region 4.1:
  //   xc_4 <= x1 <= 1
  //   -x0 * y0 <= x1 * y1
  //   (x1 - x0) * (y1 - y0) <= -x0 * y0
  //     xc_4 = (1 + phi) * x0 <= 1
  //     x0 <= phi
  //     Region 4.1.1: -x0 * y0 <= x1 * y1
  //       yc_411 = -x0 * y0
  //       Region 4.1.1.1: yc_411 <= y0 - 1
  //         -x0 * y0 <= y0 - 1
  //         (1 - y0) / y0 <= x0
  //         phi = 1 / (1 + phi) <= 1 / (1 + x0) <= y0
  auto f4111_3 = IntegrationAB(f4_4, 3, y0 - one, 0.);
  cout << "\tf4111_3 = " << f4111_3 << endl;
  auto f4111_2 = IntegrationAB(f4111_3, 2, (1 + phi) * x0, 1.);
  cout << "\tf4111_2 = " << f4111_2 << endl;
  // auto f4111_1 =
  //     IntegrationAB(f4111_2, 0, 0., DMakeFXi(1, 1., -1) / DMakeFXi(1));
  auto f4111_1 =
      IntegrationAB(f4111_2, 0, DMakeFXi(1, 1., -1) / DMakeFXi(1), phi);
  cout << "\tf4111_1 = " << f4111_1 << endl;
  auto f4111_0 = IntegrationAB(f4111_1, 1, phi, 1.0);
  cout << "\tf4111_0 = " << f4111_0 << endl;
  //       Region 4.1.1.2: y0 - 1 <= yc_411
  //         y0 - 1 <= -x0 * y0
  //         x0 <= (1 - y0) / y0
  //         y0 <= 1 / (1 + x0)
  //         Region 4.1.1.2.1: Ignore y0 - 1 <= y1
  auto f41121_3 = IntegrationAB(f4_4, 3, -x0 * y0 / x1, 0.);
  cout << "\tf41121_3 = " << f41121_3 << endl;
  auto f41121_2 = IntegrationAB(f41121_3, 2, DMakeFXi(0, 0., 1 + phi), 1.);
  cout << "\tf41121_2 = " << f41121_2 << endl;
  auto f41121_1 =
      IntegrationAB(f41121_2, 0, 0., DMakeFXi(1, 1., -1) / DMakeFXi(1));
  cout << "\tf41121_1 = " << f41121_1 << endl;
  //           Substitute1: x0s = 1 + x0, x0 = x0s - 1
  // auto f41121_2s = SubstitutionShift(f41121_2, 0, -1.);
  // cout << "\tf41121_2s = " << f41121_2s << endl;
  // auto f41121_1s =
  //     IntegrationAB(f41121_2s, 1, 0., DFactorized(1.) / DMakeFXi(0));
  // cout << "\tf41121_1s = " << f41121_1s << endl;
  auto f41121_0 = IntegrationAB(f41121_1, 1, 0., 1.);
  cout << "\tf41121_0 = " << f41121_0 << endl;
  //         Region 4.1.1.2.2 (c): y1 <= y0 - 1
  //           -phi y0 <= y1 <= y0 - 1
  //           1 <= (1 + phi) y0
  //           phi <= y0
  auto f41122_3 = IntegrationAB(f4_4, 3, -x0 * y0 / x1, y0 - one);
  cout << "\tf41122_3 = " << f41122_3 << endl;
  auto f41122_2 =
      IntegrationAB(f41122_3, 2, DMakeFXi(0, 0., 1 + phi),
                    DMakeFXi(0) * DMakeFXi(1) / DMakeFXi(1, 1., -1));
  cout << "\tf41122_2 = " << f41122_2 << endl;
  // auto f41122_2s = SubstitutionShift(f41122_2, 0, -1.);
  // cout << "\tf41122_2s = " << f41122_2s << endl;
  // auto f41122_1s =
  //     IntegrationAB(f41122_2s, 1, phi, DFactorized(1.) / DMakeFXi(0));
  // cout << "\tf41121_1s = " << f41122_1s << endl;
  auto f41122_1 =
      IntegrationAB(f41122_2, 0, 0., DMakeFXi(1, 1., -1) / DMakeFXi(1));
  cout << "\tf41122_1 = " << f41122_1 << endl;
  auto f41122_0 = IntegrationAB(f41122_1, 1, 0., 1.);
  cout << "\tf41122_0 = " << f41122_0 << endl;
  auto f4112_0 = f41121_0 - f41122_0;
  cout << "\tf4112_0 = " << f4112_0 << endl;
  auto f411_0 = f4111_0 + f4112_0;
  cout << "\tf411_0 = " << f411_0 << endl;
  //...
  auto f412_0 = zero;  // TODO
  cout << "\tf412_0 = " << f412_0 << endl;
  auto f41_0 = f411_0 - f412_0;
  cout << "\tf41_0 = " << f41_0 << endl;
  //   Region 4.2:
  //   x0 <= x1 <= xc_4
  //   x1 * y1 <= -x0 * y0
  //   -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //     Similar to Region 4.1 under (x0 <-> y0) and (x1 <-> y1) swap
  auto f42_0 = f41_0;
  auto f4_0 = f41_0 + f42_0;
  cout << "\tf4_0 = " << f4_0 << endl;

  auto f0 = f1_0 + f2_0 + f3_0 + f4_0;
  cout << "\tf0 = " << f0 << endl;
  return 0;
}
