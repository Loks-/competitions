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
  // auto fc = x0 * y0;
  // auto fc = 8. * x0 * y0 * (1. - x0) * (1. - y0);
  // 0 <= x0 <= 1, 0 <= y0 <= 1
  // Segment 1: 0 <= x1 <= x0, y0 <= y1 <= 1
  auto s1p = fc * (one - x0) * (one - y1);
  //   xc_1 = (1 - phi) * x0, yc_1 = (2 + phi) * y0
  auto xc_1 = (1 - phi) * x0, yc_1 = (2 + phi) * y0;
  //   Region 1.1:
  //     0 <= x1 <= xc_1
  //     x1 * y1 <= x0 * y0
  //     (x1 - x0) * (y1 - y0) <= -x0 * y0
  //     yc_11 = 2 * y0
  auto yc_11 = 2.0 * y0;
  //     Region 1.1.1: 1 <= yc_11
  //       0.5 <= y0
  auto f1_111 = zero;
  //     Region 1.1.2: yc_11 < 1 <= yc_1
  //       (1 - phi) <= y0 <= 0.5
  //       Substitute1 y1 = y0 + y1s
  //       After S1:
  //         0 <= y1s <= 1 - y0
  //         (x1 - x0) * y1s <= -x0 * y0
  auto s1ps1 = SubstitutionFunction(s1p, 3, y0 + y1);
  //       Substitute2 y0 = 1 - y0s
  //       After S2:
  //         0.5 <= y0s <= phi
  //         0 <= y1s <= y0s
  //         (x1 - x0) * y1s <= -x0 * (1 - y0s)
  auto s1ps2 = SubstitutionFunction(s1ps1, 1, one - y0);
  auto f4_112 = s1ps2;
  cout << "\tf4_112 = " << f4_112 << endl;
  auto f3_112 = Integration(f4_112, 2, zero, x0 - x0 * (one - y0) / y1);
  cout << "\tf3_112 = " << f3_112 << endl;
  auto f3_112_ii = Integration(f3_112, 3);
  cout << "\tf3_112_ii = " << f3_112_ii << endl;
  auto f2_112_b = SubstitutionIndex(f3_112_ii, 3, 1);
  cout << "\tf2_112_b = " << f2_112_b << endl;
  auto a_112_f3 = -DMakeFXi(1, 1);
  auto f2_112_a = SubstitutionFactorized(f3_112_ii, 3, a_112_f3);
  cout << "\tf2_112_a = " << f2_112_a << endl;
  auto f2_112 = f2_112_b - f2_112_a;
  cout << "\tf2_112 = " << f2_112 << endl;
  auto f1_112 = Integration(f2_112, 1, 0.5, phi, true);
  cout << "\tf1_112 = " << f1_112 << endl;
  //     Region 1.1.3: yc_1 < 1
  //       y0 < (1.0 - phi)
  //       Region 1.1.3.1: y1 <= yc1
  //         Using same substitute1 as region 1.1.2
  //         y1s <= yc_1 - y0 = (1 + phi) * y0
  auto f4_1131 = s1ps1;
  cout << "\tf4_1131 = " << f4_1131 << endl;
  auto f3_1131 = Integration(f4_1131, 2, zero, x0 - x0 * y0 / y1);
  cout << "\tf3_1131 = " << f3_1131 << endl;
  auto f3_1131_ii = Integration(f3_1131, 3);
  cout << "\tf3_1131_ii = " << f3_1131_ii << endl;
  auto b_1131_f3 = DMakeFXi(1) * (1.0 + phi);
  auto f2_1131_b = SubstitutionFactorized(f3_1131_ii, 3, b_1131_f3);
  cout << "\tf2_1131_b = " << f2_1131_b << endl;
  auto f2_1131_a = SubstitutionIndex(f3_1131_ii, 3, 1);
  cout << "\tf2_1131_a = " << f2_1131_a << endl;
  auto f2_1131 = f2_1131_b - f2_1131_a;
  cout << "\tf2_1131 = " << f2_1131 << endl;
  //       Region 1.1.3.2: yc1 <= y1 <= 1
  auto f4_1132 = s1p;
  cout << "\tf4_1132 = " << f4_1132 << endl;
  auto f3_1132 = Integration(f4_1132, 2, zero, x0 * y0 / y1);
  cout << "\tf3_1132 = " << f3_1132 << endl;
  auto f3_1132_ii = Integration(f3_1132, 3);
  cout << "\tf3_1132_ii = " << f3_1132_ii << endl;
  auto f2_1132_b = SubstitutionValue(f3_1132_ii, 3, 1., true);
  cout << "\tf2_1132_b = " << f2_1132_b << endl;
  auto a_1132_f3 = DMakeFXi(1) * (2.0 + phi);
  auto f2_1132_a = SubstitutionFactorized(f3_1132_ii, 3, a_1132_f3);
  cout << "\tf2_1132_a = " << f2_1132_a << endl;
  auto f2_1132 = f2_1132_b - f2_1132_a;
  cout << "\tf2_1132 = " << f2_1132 << endl;
  auto f2_113 = f2_1131 + f2_1132;
  cout << "\tf2_113 = " << f2_113 << endl;
  auto f1_113 = Integration(f2_113, 1, 0., 1. - phi, true);
  cout << "\tf1_113 = " << f1_113 << endl;
  auto f1_11 = f1_111 + f1_112 + f1_113;
  cout << "\tf1_11 = " << f1_11 << endl;
  // Segment 2: x0 <= x1 <= 1, y0 <= y1 <= 1
  auto s2p = fc * (one - x1) * (one - y1);
  //   Substitute1 x1 = x0 + x1s, y1 = y0 + y1s
  //   After S1: 0 <= x1s <= 1 - x0, 0 <= y1s <= 1 - y0
  auto s2ps1 =
      SubstitutionFunction(SubstitutionFunction(s2p, 2, x0 + x1), 3, y0 + y1);
  //   Substitute2 x0 = 1 - x0s, y0 = 1 - y0s
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
  // a = (one - x0) * (one - y0) / y0, b = x0
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
  // cout << Project(f3_212c_ii, 0, 1.) << endl;
  // Segment 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  auto s3p = fc * (1. - x1) * (1. - y0);
  // Segment 4: x0 <= x1 <= 1, y0 - 1 <= y1 <= 0
  auto s4p = fc * (1. - x1) * (1. + y1 - y0);
  return 0;
}
