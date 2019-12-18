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
  PGeometric(const TValue& _a) : a(_a) {}
  TValue Get(uint64_t n) override { return a.PowU(n); }
};

template <class TValue>
class PGeometricOne : public GeneratingFunction<TValue> {
 public:
  TValue Get(uint64_t n) override { return TValue(1); }
};

template <class TValue>
class Geometric : public GeneratingFunction<TValue> {
 protected:
  std::vector<TValue> va;

 public:
  Geometric(const TValue& a) : va({TValue(1), a}) {}

  void Adjust(uint64_t n) override {
    while (va.size() <= n) va.push_back(va.back() * va[1]);
  }

  TValue Get(uint64_t n) override {
    Adjust(n);
    return va[n];
  }
};

template <class TValue>
inline PGeneratingFunction<TValue> CreatePGeometric(const TValue& a) {
  return std::make_shared<PGeometric<TValue>>(a);
}

template <class TValue>
inline PGeneratingFunction<TValue> CreatePGeometricOne() {
  return std::make_shared<PGeometricOne<TValue>>();
}

template <class TValue>
inline PGeneratingFunction<TValue> CreateGeometric(const TValue& a) {
  return std::make_shared<Geometric<TValue>>(a);
}
}  // namespace functions
}  // namespace gf
