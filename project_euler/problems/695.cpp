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
#include <iomanip>

using namespace calculus::ext_polynomial;

using DF4 = DMVFunction<4>;

int main_695() {
  double phi = (sqrt(5.) - 1.) / 2;
  auto zero = DF4(), one = DF4(1.), x0 = DMakeXi<4>(0), y0 = DMakeXi<4>(1),
       x1 = DMakeXi<4>(2), y1 = DMakeXi<4>(3);
  // auto fc = 24. * one;
  auto fc = 24. * x0 * y0;

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
  //       Substitute: y1 = y0 + y1s
  //         y1s = y1 - y0
  //         y0 <= y1s <= 1 - y0
  //         (x1 - x0) * y1s <= -x0 * y0
  auto f111_4 = SubstitutionFunction(f1_4, 3, y0 + y1);
  auto f111_3 = IntegrationAB(f111_4, 2, 0., x0 - x0 * y0 / y1);
  //       Region 1.1.1.1: 1 <= yc_1
  //         1 <= (2 + phi) * y0
  //         1 - phi <= y0
  auto f1111_2 = IntegrationAB(f111_3, 3, y0, DMakeFXi(1, 1., -1.));
  auto f1111_1 = IntegrationAB(f1111_2, 1, 1 - phi, 0.5);
  //       Region 1.1.1.2: yc_1 < 1
  //         y0 < 1 - phi
  auto f1112_2 = IntegrationAB(f111_3, 3, y0, DMakeFXi(1, 0., 1 + phi));
  auto f1112_1 = IntegrationAB(f1112_2, 1, 0., 1 - phi);
  auto f111_1 = f1111_1 + f1112_1;
  //     Region 1.1.2: yc_1 < y1
  //       x1 * y1 <= x0 * y0
  //       (2 + phi) * y0 < y1 <= 1
  //       y0 < 1 - phi
  auto f112_3 = IntegrationAB(f1_4, 2, 0., x0 * y0 / y1);
  auto f112_2 = IntegrationAB(f112_3, 3, DMakeFXi(1, 0, 2 + phi), 1.);
  auto f112_1 = IntegrationAB(f112_2, 1, 0., 1 - phi);
  auto f11_1 = f111_1 + f112_1;
  //   Region 1.2:
  //   xc_1 <= x1 <= x0
  //   x0 * y0 <= x1 * y1
  //   -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //     Region 1.2.1: y1 <= yc_1
  //       x0 * y0 <= x1 * y1
  //       x1 <= 1
  auto f121_3 = IntegrationAB(f1_4, 2, x0 * y0 / y1, x0);
  //       Region 1.2.1.1: 1 <= yc_1
  //         1 <= (2 + phi) * y0
  //         1 - phi <= y0
  auto f1211_2 = IntegrationAB(f121_3, 3, y0, 1.);
  auto f1211_1 = IntegrationAB(f1211_2, 1, 1 - phi, 1.0);
  //       Region 1.2.1.2: yc_1 < 1
  //         y0 < 1 - phi
  auto f1212_2 = IntegrationAB(f121_3, 3, y0, DMakeFXi(1, 0., 2 + phi));
  auto f1212_1 = IntegrationAB(f1212_2, 1, 0., 1 - phi);
  auto f121_1 = f1211_1 + f1212_1;
  //     Region 1.2.2: yc_1 < y1
  //       -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //       (2 + phi) * y0 < y1 <= 1
  //       y0 < 1 - phi
  //       Substitute: y1 = y0 + y1s
  //         y1s = y1 - y0
  //         (1 + phi) * y0 <= y1s <= 1 - y0
  //         -x0 * y0 <= (x1 - x0) * y1s
  auto f122_4 = SubstitutionFunction(f1_4, 3, y0 + y1);
  auto f122_3 = IntegrationAB(f122_4, 2, x0 - x0 * y0 / y1, x0);
  auto f122_2 =
      IntegrationAB(f122_3, 3, DMakeFXi(1, 0, 1 + phi), DMakeFXi(1, 1., -1));
  auto f122_1 = IntegrationAB(f122_2, 1, 0., 1 - phi);
  auto f12_1 = f121_1 + f122_1;
  auto f1_1 = f11_1 + f12_1;
  auto f1_0 = IntegrationAB(f1_1, 0, 0., 1.);

  // Region 2: x0 <= x1 <= 1, y0 <= y1 <= 1
  //   (x1 - x0) * (y1 - y0) <= x0 * y0
  auto f2_4 = fc * (one - x1) * (one - y1);
  //   Substitute: x1 = x0 + x1s, y1 = y0 + y1s
  //     0 <= x1s <= 1 - x0, 0 <= y1s <= 1 - y0
  //     x1s * y1s <= x0 * y0
  auto f2s_4 =
      SubstitutionFunction(SubstitutionFunction(f2_4, 2, x0 + x1), 3, y0 + y1);
  //   Region 2.1: Ignore x1s * y1s <= x0 * y0
  auto f21_3 = IntegrationAB(f2s_4, 3, 0., one - y0);
  auto f21_2 = IntegrationAB(f21_3, 2, 0., one - x0);
  auto f21_1 = IntegrationAB(f21_2, 1, 0., 1.);
  //    Region 2.2 (s): x0 * y0 < x1s * y1s
  //      x0 + y0 < 1
  auto f22_3 = IntegrationAB(f2s_4, 3, x0 * y0 / x1, one - y0);
  auto f22_2 =
      IntegrationAB(f22_3, 2, DMakeFXi(0) * DMakeFXi(1) / DMakeFXi(1, 1., -1.),
                    DMakeFXi(0, 1., -1.));
  auto f22_1 = IntegrationAB(f22_2, 1, 0., DMakeFXi(0, 1., -1.));
  auto f2_1 = f21_1 - f22_1;
  auto f2_0 = IntegrationAB(f2_1, 0, 0., 1.);

  // Region 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  //   Similar to Region 1 under (x0 <-> y0) and (x1 <-> y1) swap
  auto f3_0 = f1_0;

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
  auto f4111_2 = IntegrationAB(f4111_3, 2, (1 + phi) * x0, 1.);
  auto f4111_1 =
      IntegrationAB(f4111_2, 0, DMakeFXi(1, 1., -1) / DMakeFXi(1), phi);
  auto f4111_0 = IntegrationAB(f4111_1, 1, phi, 1.0);
  //       Region 4.1.1.2: y0 - 1 <= yc_4
  //         y0 - 1 <= -phi * y0
  //         y0 <= phi
  auto f4112_3 = IntegrationAB(f4_4, 3, -x0 * y0 / x1, 0.);
  auto f4112_2 = IntegrationAB(f4112_3, 2, DMakeFXi(0, 0., 1 + phi), 1.);
  auto f4112_1 = IntegrationAB(f4112_2, 1, 0., phi);
  auto f4112_0 = IntegrationAB(f4112_1, 0, 0., phi);
  //       Region 4.1.1.3: yc_4 <= y0 - 1 <= yc_411
  //         -phi * y0 <= y0 - 1
  //         phi <= y0
  //         y0 - 1 <= -x0 * y0
  //         x0 <= (1 - y0) / y0
  //         xc_4113 = x0 * y0 / (1-y0)
  auto f41131_2 =
      IntegrationAB(f4111_3, 2, (1 + phi) * x0,
                    DMakeFXi(0) * DMakeFXi(1) / DMakeFXi(1, 1.0, -1.0));
  auto f41132_2 = IntegrationAB(
      f4112_3, 2, DMakeFXi(0) * DMakeFXi(1) / DMakeFXi(1, 1.0, -1.0), 1.0);
  auto f4113_2 = f41131_2 + f41132_2;
  auto f4113_1 =
      IntegrationAB(f4113_2, 0, 0., DMakeFXi(1, 1., -1) / DMakeFXi(1));
  auto f4113_0 = IntegrationAB(f4113_1, 1, phi, 1.0);
  auto f411_0 = f4111_0 + f4112_0 + f4113_0;
  //     Region 4.1.2 (c): -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //       (1 + phi) * x0 <= x1 <= 2 * x0
  //       Region 4.1.2.1: y0 - 1 <= yc_4
  //         y0 <= phi
  //         Substitute: x1 = x0 + x1s
  //           x1s = x1 - x0
  //           -x0 * y0 <= x1s * (y1 - y0)
  //           phi * x0 <= x1s <= x0
  auto f4121_4 = SubstitutionFunction(f4_4, 2, x0 + x1);
  auto f4121_3 = IntegrationAB(f4121_4, 3, y0 - x0 * y0 / x1, 0.);
  auto f4121_2 = IntegrationAB(f4121_3, 1, 0., phi);
  //         Region 4.1.2.1.1: 2*x0 <= 1
  //           phi * x0 <= x1s <= x0
  auto f41211_1 = IntegrationAB(f4121_2, 2, DMakeFXi(0, 0., phi), x0);
  auto f41211_0 = IntegrationAB(f41211_1, 0, 0., 0.5);
  //         Region 4.1.2.1.2: (1 + phi) * x0 <= 1 <= 2*x0
  //           (1 + phi) * x0 <= x1 <= 1
  //           phi * x0 <= x1s <= 1 - x0
  //           0.5 <= x0 <= phi
  auto f41212_1 =
      IntegrationAB(f4121_2, 2, DMakeFXi(0, 0., phi), DMakeFXi(0, 1., -1.));
  auto f41212_0 = IntegrationAB(f41212_1, 0, 0.5, phi);
  auto f4121_0 = f41211_0 + f41212_0;
  //       Region 4.1.2.2: yc_4 <= y0 - 1
  //         phi <= y0
  //         xc_4122 = x0 * (y0 + 1)
  //         Region 4.1.2.2.1: x1 <= xc_4122
  auto f41221_3 = IntegrationAB(f4_4, 3, y0 - one, 0.);
  //         Region 4.1.2.2.1.1: xc_4122 <= 1
  //           (1 + phi) * x0 <= x1 <= xc_4122
  //           x0 * (y0 + 1) <= 1
  //           y0 <= (1 - x0) / x0
  auto f412211_2 = IntegrationAB(f41221_3, 2, (1 + phi) * x0, x0 * (y0 + 1.));
  //         Region 4.1.2.2.1.1.1: x0 <= 0.5
  //           y0 <= 1
  auto f4122111_1 = IntegrationAB(f412211_2, 1, phi, 1.0);
  auto f4122111_0 = IntegrationAB(f4122111_1, 0, 0., 0.5);
  //         Region 4.1.2.2.1.1.2: 0.5 <= x0 <= phi
  //           y0 <= (1 - x0) / x0
  auto f4122112_1 = IntegrationAB(f412211_2, 1, phi, (one - x0) / x0);
  auto f4122112_0 = IntegrationAB(f4122112_1, 0, 0.5, phi);
  auto f412211_0 = f4122111_0 + f4122112_0;
  //         Region 4.1.2.2.1.2: 1 <= xc_4122
  //           (1 + phi) x0 <= x1 <= 1
  //           1 <= x0 * (y0 + 1)
  //           (1 - x0) / x0 <= y0 <= 1
  auto f412212_2 = IntegrationAB(f41221_3, 2, (1 + phi) * x0, 1.);
  auto f412212_1 = IntegrationAB(f412212_2, 1, (one - x0) / x0, 1.);
  auto f412212_0 = IntegrationAB(f412212_1, 0, 0.5, phi);
  auto f41221_0 = f412211_0 + f412212_0;
  //         Region 4.1.2.2.2: xc_4122 <= x1
  //           -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //           Substitute: x1 = x0 + x1s
  //             x1s = x1 - x0 <= 1 - x0
  //             -x0 * y0 <= x1s * (y1 - y0)
  //             x0 * (y0 + 1) <= x1s + x0
  //             x0 * y0 <= x1s
  auto f41222_3 = f4121_3;
  //           Region 4.1.2.2.2.1: 2*x0 <= 1
  //             x0 * y0 <= x1s <= x0
  auto f412221_2 = IntegrationAB(f41222_3, 2, DMakeFXi(0) * DMakeFXi(1), x0);
  auto f412221_1 = IntegrationAB(f412221_2, 1, phi, 1.0);
  auto f412221_0 = IntegrationAB(f412221_1, 0, 0., 0.5);
  //           Region 4.1.2.2.2.2: 1 <= 2*x0
  //             x0 * y0 <= x1s <= 1 - x0
  //             x0 * (y0 + 1) <= x1 <= 1
  //             y0 <= (1 - x0) / x0
  auto f412222_2 = IntegrationAB(f41222_3, 2, DMakeFXi(0) * DMakeFXi(1),
                                 DMakeFXi(0, 1., -1.));
  auto f412222_1 =
      IntegrationAB(f412222_2, 1, phi, DMakeFXi(0, 1., -1.) / DMakeFXi(0));
  auto f412222_0 = IntegrationAB(f412222_1, 0, 0.5, phi);
  auto f41222_0 = f412221_0 + f412222_0;
  auto f4122_0 = f41221_0 + f41222_0;
  auto f412_0 = f4121_0 + f4122_0;
  auto f41_0 = f411_0 - f412_0;
  //   Region 4.2:
  //   x0 <= x1 <= xc_4
  //   x1 * y1 <= -x0 * y0
  //   -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //     Similar to Region 4.1 under (x0 <-> y0) and (x1 <-> y1) swap
  auto f42_0 = f41_0;
  auto f4_0 = f41_0 + f42_0;
  auto f0 = f1_0 + f2_0 + f3_0 + f4_0;

  cout << setprecision(10) << fixed << f0.ToValue() << endl;
  return 0;
}
