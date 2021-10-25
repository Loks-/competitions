#pragma once

#include "common/base.h"
#include "common/generating_function/functions/constant.h"
#include "common/generating_function/generating_function.h"
#include "common/generating_function/operators/multiplication.h"

namespace gf {
namespace operators {
template <class TValue>
inline PGeneratingFunction<TValue> MakePowU(PGeneratingFunction<TValue> f,
                                            uint64_t pow);

template <class TValue>
class PowU : public GeneratingFunction<TValue> {
 protected:
  PGeneratingFunction<TValue> f, t;
  uint64_t pow;

 public:
  PowU(PGeneratingFunction<TValue> _f, uint64_t _pow) : pow(_pow) {
    if (pow == 0) {
      f = gf::functions::MakePConstant<TValue>(TValue(1));
    } else if (pow == 1) {
      f = _f;
    } else if (pow & 1) {
      t = MakePowU<TValue>(_f, pow - 1);
      f = MakeMultiplication<TValue>(_f, t);
    } else {
      t = MakePowU<TValue>(_f, pow / 2);
      f = MakeMultiplication<TValue>(t, t);
    }
  }

  void Adjust(uint64_t n) override { f->Adjust(n); }

  TValue Get(uint64_t n) override { return f->Get(n); }
};

template <class TValue>
inline PGeneratingFunction<TValue> MakePowU(PGeneratingFunction<TValue> f,
                                            uint64_t pow) {
  return std::make_shared<PowU<TValue>>(f, pow);
}
}  // namespace operators
}  // namespace gf
