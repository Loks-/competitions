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
#include "common/stl/base.h"

using namespace calculus::ext_polynomial;

using DF4 = DMVFunction<4>;

int main_695() {
  auto zero = DF4(), one = DF4(1.), x0 = DMakeXi<4>(0), y0 = DMakeXi<4>(1),
       x1 = DMakeXi<4>(2), y1 = DMakeXi<4>(3);
  auto fc = one;
  // auto fc = x0 * y0;
  // auto fc = 8. * x0 * y0 * (1. - x0) * (1. - y0);
  // 0 <= x0 <= 1, 0 <= y0 <= 1
  // Segment 1: 0 <= x1 <= x0, y0 <= y1 <= 1
  auto s1p = fc * (one - x0) * (one - y1);
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
  cout << "s2ps2 = " << s2ps2 << endl;
  //   Region 2.1: (x1 - x0) * (y1 - y0) <= x0 * y0
  //     After S1: x1s * y1s <= x0 * y0
  //     After S2: x1s * y1s <= (1 - x0s) * (1 - y0s)
  //     Region 2.1.1: 1 <= x0 + y0
  //       After S2: x0s + y0s <= 1
  auto f4_211 = s2ps2;
  auto f3_211 = Integration(f4_211, 3, zero, y0);
  auto f2_211 = Integration(f3_211, 2, zero, x0);
  cout << "f2_211 = " << f2_211 << endl;
  auto f1_211 = Integration(f2_211, 1, zero, one - x0);
  cout << "f1_211 = " << f1_211 << endl;
  //     Region 2.1.2: x0 + y0 <= 1
  //       After S2: 1 <= x0s + y0s
  auto f4_212 = s2ps2;
  auto f3_212c = Integration(f4_212, 3, (one - x0) * (one - y0) / x1, y0);
  cout << "f3_212c = " << f3_212c << endl;
  // a = (one - x0) * (one - y0) / y0, b = x0
  auto f3_212c_ii = Integration(f3_212c, 2);
  cout << "f3_212c_ii = " << f3_212c_ii << endl;
  auto f2_212c_b = SubstitutionIndex(f3_212c_ii, 2, 0);
  cout << "f2_212c_b = " << f2_212c_b << endl;
  auto a_212c_f = DMakeFXi(0, 1.) * DMakeFXi(1, 1.) / DMakeFXi(1);
  auto f2_212c_a = SubstitutionFactorized(f3_212c_ii, 2, a_212c_f);
  cout << "f2_212c_a = " << f2_212c_a << endl;
  auto f2_212c = f2_212c_b - f2_212c_a;
  auto f2_212 = f2_211 - f2_212c;
  cout << "f2_212 = " << f2_212 << endl;
  auto f1_212 = Integration(f2_212, 1, one - x0, one);
  auto f1_2 = f1_211 + f1_212;
  auto f0_2 = Integration(f1_2, 0, zero, one);
  cout << "f0_2 = " << f0_2 << endl;
  // cout << Project(f3_212c_ii, 0, 1.) << endl;
  // Segment 3: x0 <= x1 <= 1, 0 <= y1 <= y0
  auto s3p = fc * (1. - x1) * (1. - y0);
  // Segment 4: x0 <= x1 <= 1, y0 - 1 <= y1 <= 0
  auto s4p = fc * (1. - x1) * (1. + y1 - y0);
  return 0;
}
