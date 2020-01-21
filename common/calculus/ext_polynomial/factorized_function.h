#pragma once

#include "common/calculus/ext_polynomial/factorized.h"
#include "common/calculus/ext_polynomial/factorized_conversion.h"
#include "common/calculus/ext_polynomial/mv_division_linear.h"
#include "common/calculus/ext_polynomial/mv_function.h"
#include <vector>

namespace calculus {
namespace ext_polynomial {
template <class TValue, unsigned dim>
class FactorizedFunction {
 public:
  using TFactorized = Factorized<TValue>;
  using TFunction = MVFunction<TValue, dim>;
  using TSelf = FactorizedFunction<TValue, dim>;

  TFactorized f1;
  TFunction f2;

  FactorizedFunction() {}
  FactorizedFunction(const TFactorized& _f1) : f1(_f1), f2(TValue(1)) {}
  FactorizedFunction(const TFunction& _f2) : f2(_f2) {}
  FactorizedFunction(const TFactorized& _f1, const TFunction& _f2)
      : f1(_f1), f2(_f2) {}

  void Compress(bool force_compression) {
    f1.Compress();
    if (f1.a != TValue(1)) {
      f2 *= f1.a;
      f1.a = TValue(1);
    }
    for (auto& l : f1.vn) f2 *= ToFunction<TValue, dim>(l);
    f1.vn.clear();
    if (f2.Empty()) f1.vd.clear();
    std::vector<typename TFactorized::TLinear> vdt;
    for (auto& l : f1.vd) {
      auto ft = DivisionLinearV(f2, l.index, l.c, force_compression);
      if (!ft.Empty()) {
        f2 = ft;
      } else {
        vdt.push_back(l);
      }
    }
    f1.vd.swap(vdt);
  }
};
}  // namespace ext_polynomial
}  // namespace calculus
