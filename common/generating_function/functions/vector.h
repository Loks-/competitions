#pragma once

#include "common/generating_function/generating_function.h"

#include <vector>

namespace gf {
namespace functions {
template <class TValue>
class PVector : public GeneratingFunction<TValue> {
 protected:
  std::vector<TValue> v;

 public:
  explicit PVector(const std::vector<TValue>& _v) : v(_v) {}
  TValue Get(uint64_t n) override { return (n < v.size()) ? v[n] : TValue(0); }
};

template <class TValue>
inline PGeneratingFunction<TValue> MakePVector(const std::vector<TValue>& v) {
  return std::make_shared<PVector<TValue>>(v);
}
}  // namespace functions
}  // namespace gf
