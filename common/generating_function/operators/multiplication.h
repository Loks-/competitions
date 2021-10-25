#pragma once

#include "common/base.h"
#include "common/generating_function/generating_function.h"

#include <vector>

namespace gf {
namespace operators {
template <class TValue>
class Multiplication : public GeneratingFunction<TValue> {
 protected:
  PGeneratingFunction<TValue> f1, f2;
  std::vector<TValue> va;

 public:
  Multiplication(PGeneratingFunction<TValue> _f1,
                 PGeneratingFunction<TValue> _f2)
      : f1(_f1), f2(_f2) {}

  void Adjust(uint64_t n) override {
    if (n >= va.size()) {
      f1->Adjust(n);
      f2->Adjust(n);
      for (unsigned k = va.size(); k <= n; ++k) {
        TValue x = TValue();
        for (unsigned i = 0; i <= k; ++i) x += f1->Get(i) * f2->Get(k - i);
        va.push_back(x);
      }
    }
  }

  TValue Get(uint64_t n) override {
    Adjust(n);
    return va[n];
  }
};

template <class TValue>
inline PGeneratingFunction<TValue> MakeMultiplication(
    PGeneratingFunction<TValue> f1, PGeneratingFunction<TValue> f2) {
  return std::make_shared<Multiplication<TValue>>(f1, f2);
}
}  // namespace operators

template <class TValue>
inline PGeneratingFunction<TValue> operator*(PGeneratingFunction<TValue> f1,
                                             PGeneratingFunction<TValue> f2) {
  return operators::MakeMultiplication(f1, f2);
}
}  // namespace gf
