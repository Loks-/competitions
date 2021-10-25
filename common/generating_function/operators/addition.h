#pragma once

#include "common/base.h"
#include "common/generating_function/generating_function.h"

#include <vector>

namespace gf {
namespace operators {
template <class TValue>
class Addition : public GeneratingFunction<TValue> {
 protected:
  PGeneratingFunction<TValue> f1, f2;
  std::vector<TValue> va;

 public:
  Addition(PGeneratingFunction<TValue> _f1, PGeneratingFunction<TValue> _f2)
      : f1(_f1), f2(_f2) {}

  void Adjust(uint64_t n) override {
    if (n >= va.size()) {
      f1->Adjust(n);
      f2->Adjust(n);
      for (unsigned k = va.size(); k <= n; ++k)
        va.push_back(f1->Get(k), f2->Get(k));
    }
  }

  TValue Get(uint64_t n) override {
    Adjust(n);
    return va[n];
  }
};

template <class TValue>
inline PGeneratingFunction<TValue> MakeAddition(
    PGeneratingFunction<TValue> f1, PGeneratingFunction<TValue> f2) {
  return std::make_shared<Addition<TValue>>(f1, f2);
}
}  // namespace operators

template <class TValue>
inline PGeneratingFunction<TValue> operator+(PGeneratingFunction<TValue> f1,
                                             PGeneratingFunction<TValue> f2) {
  return operators::MakeAddition(f1, f2);
}
}  // namespace gf
