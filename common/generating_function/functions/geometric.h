#pragma once

#include "common/generating_function/generating_function.h"

#include <vector>

namespace gf {
namespace functions {
template <class TValue>
class PGeometric : public GeneratingFunction<TValue> {
 protected:
  TValue a;

 public:
  explicit PGeometric(const TValue& _a) : a(_a) {}
  TValue Get(uint64_t n) override { return a.PowU(n); }
};

template <class TValue>
class PGeometricOne : public GeneratingFunction<TValue> {
 public:
  TValue Get(uint64_t) override { return TValue(1); }
};

template <class TValue>
class Geometric : public GeneratingFunction<TValue> {
 protected:
  std::vector<TValue> va;

 public:
  explicit Geometric(const TValue& a) : va({TValue(1), a}) {}

  void Adjust(uint64_t n) override {
    while (va.size() <= n) va.push_back(va.back() * va[1]);
  }

  TValue Get(uint64_t n) override {
    Adjust(n);
    return va[n];
  }
};

template <class TValue>
inline PGeneratingFunction<TValue> MakePGeometric(const TValue& a) {
  return std::make_shared<PGeometric<TValue>>(a);
}

template <class TValue>
inline PGeneratingFunction<TValue> MakePGeometricOne() {
  return std::make_shared<PGeometricOne<TValue>>();
}

template <class TValue>
inline PGeneratingFunction<TValue> MakeGeometric(const TValue& a) {
  return std::make_shared<Geometric<TValue>>(a);
}
}  // namespace functions
}  // namespace gf
