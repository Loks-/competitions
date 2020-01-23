#include "common/calculus/ext_polynomial/division.h"
#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/io.h"
#include "common/calculus/ext_polynomial/multiplication.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include "common/calculus/ext_polynomial/mv_integration.h"
#include "common/calculus/ext_polynomial/mv_make.h"
#include "common/calculus/ext_polynomial/mv_substitution_factorized.h"
#include "common/calculus/ext_polynomial/mv_substitution_function.h"
#include "common/calculus/ext_polynomial/mv_substitution_index.h"
#include "common/calculus/ext_polynomial/mv_substitution_value.h"
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
  // Segment 1: 0 <= x1 <= x0, y0 <= y1 <= 1
  auto s1p = fc * (one - x0) * (one - y1);
  //   xc_1 = (1 - phi) * x0, yc_1 = (2 + phi) * y0
  //   Region 1.1:
  //   0 <= x1 <= xc_1
  //   x1 * y1 <= x0 * y0
  //   (x1 - x0) * (y1 - y0) <= -x0 * y0
  //     yc_11 = 2 * y0
  //     Region 1.1.1: 1 <= yc_11
  //       0.5 <= y0
  auto f1_111 = zero;
  //     Region 1.1.2: yc_11 < 1 <= yc_1
  //       (1 - phi) <= y0 <= 0.5
  //       Substitute1: y1 = y0 + y1s
  //       After S1:
  //         0 <= y1s <= 1 - y0
  //         (x1 - x0) * y1s <= -x0 * y0
  auto s1ps1 = SubstitutionFunction(s1p, 3, y0 + y1);
  //       Substitute2: y0 = 1 - y0s
  //       After S2:
  //         0.5 <= y0s <= phi
  //         0 <= y1s <= y0s
  //         (x1 - x0) * y1s <= -x0 * (1 - y0s)
  auto s1ps2 = SubstitutionFunction(s1ps1, 1, one - y0);
  auto f4_112 = s1ps2;
  auto f3_112 = Integration(f4_112, 2, zero, x0 - x0 * (one - y0) / y1);
  auto f3_112_ii = Integration(f3_112, 3);
  auto f2_112_b = SubstitutionIndex(f3_112_ii, 3, 1);
  auto a_112_f3 = -DMakeFXi(1, 1);
  auto f2_112_a = SubstitutionFactorized(f3_112_ii, 3, a_112_f3);
  auto f2_112 = f2_112_b - f2_112_a;
  auto f1_112 = Integration(f2_112, 1, 0.5, phi, true);
  //     Region 1.1.3: yc_1 < 1
  //       y0 < (1.0 - phi)
  //       Region 1.1.3.1: y1 <= yc1
  //         Using same substitute1 as region 1.1.2
  //         y1s <= yc_1 - y0 = (1 + phi) * y0
  auto f4_1131 = s1ps1;
  auto f3_1131 = Integration(f4_1131, 2, zero, x0 - x0 * y0 / y1);
  auto f3_1131_ii = Integration(f3_1131, 3);
  auto b_1131_f3 = DMakeFXi(1) * (1.0 + phi);
  auto f2_1131_b = SubstitutionFactorized(f3_1131_ii, 3, b_1131_f3);
  auto f2_1131_a = SubstitutionIndex(f3_1131_ii, 3, 1);
  auto f2_1131 = f2_1131_b - f2_1131_a;
  //       Region 1.1.3.2: yc1 <= y1 <= 1
  auto f4_1132 = s1p;
  auto f3_1132 = Integration(f4_1132, 2, zero, x0 * y0 / y1);
  auto f3_1132_ii = Integration(f3_1132, 3);
  auto f2_1132_b = SubstitutionValue(f3_1132_ii, 3, 1., true);
  auto a_1132_f3 = DMakeFXi(1) * (2.0 + phi);
  auto f2_1132_a = SubstitutionFactorized(f3_1132_ii, 3, a_1132_f3);
  auto f2_1132 = f2_1132_b - f2_1132_a;
  auto f2_113 = f2_1131 + f2_1132;
  auto f1_113 = Integration(f2_113, 1, 0., 1. - phi, true);
  auto f1_11 = f1_111 + f1_112 + f1_113;
  cout << "\tf1_11 = " << f1_11 << endl;
  //   Region 1.2:
  //   xc_1 <= x1 <= x0
  //   x0 * y0 <= x1 * y1
  //   -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //     Region 1.2.1: 1 <= yc_1
  //       (1 - phi) <= y0
  auto f4_121 = s1p;
  cout << "\tf4_121 = " << f4_121 << endl;
  auto f3_121 = Integration(f4_121, 2, x0 * y0 / y1, x0);
  cout << "\tf3_121 = " << f3_121 << endl;
  auto f3_121_ii = Integration(f3_121, 3);
  cout << "\tf3_121_ii = " << f3_121_ii << endl;
  auto f2_121_b = SubstitutionValue(f3_121_ii, 3, 1., true);
  cout << "\tf2_121_b = " << f2_121_b << endl;
  auto f2_121_a = SubstitutionIndex(f3_121_ii, 3, 1);
  cout << "\tf2_121_a = " << f2_121_a << endl;
  auto f2_121 = f2_121_b - f2_121_a;
  cout << "\tf2_121 = " << f2_121 << endl;
  auto f1_121 = Integration(f2_121, 1, 1. - phi, 1., true);
  cout << "\tf1_121 = " << f1_121 << endl;
  //     Region 1.2.2: yc1 < 1
  //       y0 < (1 - phi)
  //       Region 1.2.2.1: y1 <= yc1
  auto f3_1221_ii = f3_121_ii;
  cout << "\tf3_1221_ii = " << f3_1221_ii << endl;
  auto b_1221_f3 = DMakeFXi(1) * (2 + phi);
  auto f2_1221_b = SubstitutionFactorized(f3_1221_ii, 3, b_1221_f3);
  cout << "\tf2_1221_b = " << f2_1221_b << endl;
  auto f2_1221_a = SubstitutionIndex(f3_1221_ii, 3, 1);
  cout << "\tf2_1221_a = " << f2_1221_a << endl;
  auto f2_1221 = f2_1221_b - f2_1221_a;
  cout << "\tf2_1221 = " << f2_1221 << endl;
  //       Region 1.2.2.2: yc1 < y1
  //       Substitute1 y1 = y0 + y1s
  //       After S1:
  //         y1s - y0 <= y1s <= 1 - y0
  //         -x0 * y0 <= (x1 - x0) * y1s
  auto f4_1222 = s1ps1;
  cout << "\tf4_1222 = " << f4_1222 << endl;
  auto f3_1222 = Integration(f4_1222, 2, x0 - x0 * y0 / y1, x0);
  cout << "\tf3_1222 = " << f3_1222 << endl;
  auto f3_1222_ii = Integration(f3_1222, 3);
  cout << "\tf3_1222_ii = " << f3_1222_ii << endl;
  auto b_1222_f3 = -DMakeFXi(1, 1);
  auto f2_1222_b = SubstitutionFactorized(f3_1222_ii, 3, b_1222_f3);
  cout << "\tf2_1222_b = " << f2_1222_b << endl;
  auto a_1222_f3 = DMakeFXi(1) * (1 + phi);
  auto f2_1222_a = SubstitutionFactorized(f3_1222_ii, 3, a_1222_f3);
  cout << "\tf2_1222_a = " << f2_1222_a << endl;
  auto f2_1222 = f2_1222_b - f2_1222_a;
  cout << "\tf2_1222 = " << f2_1222 << endl;
  auto f2_122 = f2_1221 + f2_1222;
  cout << "\tf2_122 = " << f2_122 << endl;
  auto f1_122 = Integration(f2_122, 1, 0., 1.0 - phi, true);
  cout << "\tf1_122 = " << f1_122 << endl;
  auto f1_12 = f1_121 + f1_122;
  cout << "\tf1_12 = " << f1_12 << endl;
  auto f1_1 = f1_11 + f1_12;
  cout << "\tf1_1 = " << f1_1 << endl;
  auto f0_1 = Integration(f1_1, 0, 0., 1., true);
  cout << "\tf0_1 = " << f0_1 << endl;

  // Segment 2: x0 <= x1 <= 1, y0 <= y1 <= 1
  auto s2p = fc * (one - x1) * (one - y1);
  //   Substitute1: x1 = x0 + x1s, y1 = y0 + y1s
  //   After S1: 0 <= x1s <= 1 - x0, 0 <= y1s <= 1 - y0
  auto s2ps1 =
      SubstitutionFunction(SubstitutionFunction(s2p, 2, x0 + x1), 3, y0 + y1);
  //   Substitute2: x0 = 1 - x0s, y0 = 1 - y0s
  //   After S2: 0 <= x1s <= x0s, 0 <= y1s <= y0s
  auto s2ps2 = SubstitutionFunction(SubstitutionFunction(s2ps1, 0, one - x0), 1,
                                    one - y0);
  //   Region 2.1: (x1 - x0) * (y1 - y0) <= x0 * y0
  //     After S1: x1s * y1s <= x0 * y0
  //     After S2: x1s * y1s <= (1 - x0s) * (1 - y0s)
  //     Region 2.1.1: 1 <= x0 + y0
  //       After S2: x0s + y0s <= 1
  auto f4_211 = s2ps2;
  auto f3_211 = Integration(f4_211, 3, zero, y0);
  auto f2_211 = Integration(f3_211, 2, zero, x0);
  auto f1_211 = Integration(f2_211, 1, zero, one - x0);
  //     Region 2.1.2: x0 + y0 <= 1
  //       After S2: 1 <= x0s + y0s
  auto f4_212 = s2ps2;
  auto f3_212c = Integration(f4_212, 3, (one - x0) * (one - y0) / x1, y0);
  auto f3_212c_ii = Integration(f3_212c, 2);
  auto f2_212c_b = SubstitutionIndex(f3_212c_ii, 2, 0);
  auto a_212c_f3 = DMakeFXi(0, 1.) * DMakeFXi(1, 1.) / DMakeFXi(1);
  auto f2_212c_a = SubstitutionFactorized(f3_212c_ii, 2, a_212c_f3);
  auto f2_212c = f2_212c_b - f2_212c_a;
  auto f2_212 = f2_211 - f2_212c;
  auto f2_212_ii = Integration(f2_212, 1);
  auto f1_212_b = SubstitutionValue(f2_212_ii, 1, 1., true);
  auto a_212_f2 = -DMakeFXi(0, 1);
  auto f1_212_a = SubstitutionFactorized(f2_212_ii, 1, a_212_f2);
  auto f1_212 = f1_212_b - f1_212_a;
  auto f1_2 = f1_211 + f1_212;
  auto f0_2 = Integration(f1_2, 0, 0., 1., true);
  cout << "\tf0_2 = " << f0_2 << endl;

  // Segment 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  //   Similar to Segment 1 under (x0 <-> y0) and (x1 <-> y1) swap
  auto f0_3 = f0_1;
  cout << "\tf0_3 = " << f0_3 << endl;

  // Segment 4: x0 <= x1 <= 1, y0 - 1 <= y1 <= 0
  auto s4p = fc * (1. - x1) * (1. + y1 - y0);
  //   xc_4 = (1 + phi) * x0, yc_4 = -phi * y0
  //   Region 4.1:
  //   xc_4 <= x1 <= 1
  //   -x0 * y0 <= x1 * y1
  //   (x1 - x0) * (y1 - y0) <= -x0 * y0
  //     Region 4.1.1 (c): (x1 - x0) * (y1 - y0) <= -x0 * y0
  //       Region 4.1.1.1: x1 < 1 <= 2*x0
  //         0.5 <= x0 < phi
  //       Region 4.1.1.2: 2*x0 < 1
  //         x0 <= 0.5
  auto f0_411 = zero;  // TODO.
  //     Region 4.1.2: x1 * y1 <= -x0 * y0
  //       yc_412 = -x0 * y0
  //       Region 4.1.2.1 (y0 - 1) <= yc_4
  //         (1+phi) y0 <= 1
  //         y0 <= phi
  auto f4_4121 = s4p;
  cout << "\tf4_4121 = " << f4_4121 << endl;
  auto f3_4121 = Integration(s4p, 3, -x0 * y0 / x1, zero);
  cout << "\tf3_4121 = " << f3_4121 << endl;
  auto f3_4121_ii = Integration(f3_4121, 2);
  cout << "\tf3_4121_ii = " << f3_4121_ii << endl;
  auto f2_4121_b = SubstitutionValue(f3_4121_ii, 2, 1., true);
  auto a_4121_f3 = DMakeFXi(0) * (1 + phi);
  auto f2_4121_a = SubstitutionFactorized(f3_4121_ii, 2, a_4121_f3);
  auto f2_4121 = f2_4121_b - f2_4121_a;
  cout << "\tf2_4121 = " << f2_4121 << endl;
  auto f1_4121 = Integration(f2_4121, 1, 0., phi, true);
  cout << "\tf1_4121 = " << f1_4121 << endl;
  auto f0_4121 = Integration(f1_4121, 0, 0., phi, true);
  cout << "\tf0_4121 = " << f0_4121 << endl;
  //       Region 4.1.2.2 yc4 < (y0 - 1) <= yc_412
  //         1 < (1+phi) y0
  //           phi < y0
  //         y0 * (1 + x0) <= 1
  //           y0 <= 1 / (1 + x0)
  //         Region 4.1.2.2.1
  //           xc_4 <= x1 <= 1
  //           -x0 * y0 / x1 <= y1 <= 0
  auto f2_41221 = f2_4121;
  cout << "\tf2_41221 = " << f2_41221 << endl;
  // auto f1_41221 = Integration(f2_41221, 0, (one - y0) / y0, phi * one);
  //   auto f2_41221_ii = Integration(f2_41221, 0);
  //   cout << "\tf2_41221_ii = " << f2_41221_ii << endl;
  //   auto f1_41221_b = SubstitutionValue(f2_41221_ii, 0, phi, true);
  //   auto a_41221_f2 = -DMakeFXi(1, 1) / DMakeFXi(1);
  //   auto f1_41221_a = SubstitutionFactorized(f2_41221_ii, 0, a_41221_f2);
  //   auto f1_41221 = f1_41221_b - f1_41221_a;
  //   cout << "\tf1_41221 = " << f1_41221 << endl;
  //   // auto f0_41221 = Integration(f1_41221, 1, phi, 1., true);
  //   auto f0_41221 = zero;  // TODO: Support integration
  //   cout << "\tf0_41221 = " << f0_41221 << endl;
  //         Region 4.1.2.2.2 (c)
  //           xc_4 <= x1 <= x0 * y0 / (1 - y0)
  //           -x0 * y0 / x1 <= y1 <= y0 - 1
  auto f4_41222 = s4p;
  cout << "\tf4_41222 = " << f4_41222 << endl;
  auto f3_41222 = Integration(f4_41222, 2, (1.0 + phi) * x0, -x0 * y0 / y1);
  cout << "\tf3_41222 = " << f3_41222 << endl;
  auto f3_41222_ii = Integration(f3_41222, 3);
  cout << "\tf3_41222_ii = " << f3_41222_ii << endl;
  auto b_f3_41222 = -DMakeFXi(1, 1);
  auto f2_41222_b = SubstitutionFactorized(f3_41222_ii, 3, b_f3_41222);
  auto a_f3_41222 = -DMakeFXi(1, 0) * phi;
  auto f2_41222_a = SubstitutionFactorized(f3_41222_ii, 3, a_f3_41222);
  auto f2_41222 = f2_41222_b - f2_41222_a;
  cout << "\tf2_41222 = " << f2_41222 << endl;
  auto f2_4122 = f2_41221 - f2_41222;
  cout << "\tf2_4122 = " << f2_4122 << endl;
  auto f2_4122_ii = Integration(f2_4122, 0);
  cout << "\tf2_4122_ii = " << f2_4122_ii << endl;
  auto f1_4122_b = SubstitutionValue(f2_4122_ii, 0, phi, true);
  auto a_4122_f2 = -DMakeFXi(1, 1) / DMakeFXi(1);
  auto f1_4122_a = SubstitutionFactorized(f2_4122_ii, 0, a_4122_f2);
  auto f1_4122 = f1_4122_b - f1_4122_a;
  cout << "\tf1_4122 = " << f1_4122 << endl;
  auto f0_4122 = Integration(f1_4122, 1, phi, 1., true);
  cout << "\tf0_4122 = " << f0_4122 << endl;
  //       Region 4.1.2.3 yc412 < (y0 - 1)
  //         1 < y0 * (1 + x0)
  //           1 / (1 + x0) < y0
  auto f4_4123 = s4p;
  cout << "\tf4_4123 = " << f4_4123 << endl;
  auto f3_4123 = Integration(f4_4123, 3, y0 - one, zero);
  cout << "\tf3_4123 = " << f3_4123 << endl;
  auto f2_4123 = Integration(f3_4123, 2, (1 + phi) * x0, one);
  cout << "\tf2_4123 = " << f2_4123 << endl;
  auto f1_4123 = Integration(f2_4123, 0, (one - y0) / y0, phi * one);
  cout << "\tf1_4123 = " << f1_4123 << endl;
  auto f0_4123 = Integration(f1_4123, 1, phi, 1., true);
  cout << "\tf0_4123 = " << f0_4123 << endl;
  auto f0_412 = f0_4121 + f0_4122 + f0_4123;
  cout << "\tf0_412 = " << f0_412 << endl;
  auto f0_41 = f0_412 - f0_411;
  cout << "\tf0_41 = " << f0_41 << endl;
  //   Region 4.2:
  //   x0 <= x1 <= xc_4
  //   x1 * y1 <= -x0 * y0
  //   -x0 * y0 <= (x1 - x0) * (y1 - y0)
  //     Similar to Region 4.1 under (x0 <-> y0) and (x1 <-> y1) swap
  auto f0_42 = f0_41;
  auto f0_4 = f0_41 + f0_42;
  cout << "\tf0_4 = " << f0_4 << endl;

  auto f0 = f0_1 + f0_2 + f0_3 + f0_4;
  cout << "\tf0 = " << f0 << endl;
  return 0;
}
