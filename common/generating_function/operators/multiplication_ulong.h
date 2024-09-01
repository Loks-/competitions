#pragma once

#include "common/base.h"
#include "common/generating_function/generating_function.h"
#include "common/generating_function/operators/multiplication.h"
#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/unsigned/convolution.h"

#include <vector>

namespace gf {
namespace operators {
template <>
class Multiplication<LongUnsigned> : public GeneratingFunction<LongUnsigned> {
 public:
  using TValue = LongUnsigned;

 protected:
  PGeneratingFunction<TValue> f1, f2;
  std::vector<TValue> va;

 public:
  Multiplication(PGeneratingFunction<TValue> _f1,
                 PGeneratingFunction<TValue> _f2)
      : f1(_f1), f2(_f2) {}

  void Adjust(uint64_t n) override {
    const uint64_t n0 = va.size();
    if (n < n0) return;
    f1->Adjust(n);
    f2->Adjust(n);
    if (n < n0 + 10) {
      for (unsigned k = n0; k <= n; ++k) {
        TValue x = TValue();
        for (unsigned i = 0; i <= k; ++i) x += f1->Get(i) * f2->Get(k - i);
        va.push_back(x);
      }
    } else {
      // Use convolution
      std::vector<TValue> v1(n + 1), v2(n + 1);
      for (uint64_t k = 0; k <= n; ++k) {
        v1[k] = f1->Get(k);
        v2[k] = f2->Get(k);
      }
      const auto vt = numeric::nlong::Convolution(v1, v2);
      va.resize(n + 1);
      for (unsigned k = n0; k <= n; ++k) va[k] = vt[k];
    }
  }

  TValue Get(uint64_t n) override {
    Adjust(n);
    return va[n];
  }
};
}  // namespace operators
}  // namespace gf
