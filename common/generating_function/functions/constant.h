#pragma once

#include "common/generating_function/generating_function.h"

namespace gf {
namespace functions {
template <class TValue>
class PConstant : public GeneratingFunction<TValue> {
 protected:
  TValue a0;

 public:
  explicit PConstant(const TValue& _a0) : a0(_a0) {}
  TValue Get(uint64_t n) override { return n ? TValue(0) : a0; }
};

template <class TValue>
inline PGeneratingFunction<TValue> MakePConstant(const TValue& a0) {
  return std::make_shared<PConstant<TValue>>(a0);
}
}  // namespace functions
}  // namespace gf
