#pragma once

#include "common/numeric/utils/pow.h"
#include <memory>

namespace calculus {
namespace ext_polynomial {
template <class TValue>
class TermBase {
 public:
  int power;

  TermBase(int _power) : power(_power) {}

  TValue GetTermBase(const TValue& x) const = 0;
  TValue Get(const TValue& x) const { return GetTermBase(x) * PowS(x, power); }
  bool operator<(const TermBase& r) const { return power < r.power; }
};

template <class TValue>
using PTermBase = std::shared_ptr<TermBase<TValue>>;
}  // namespace ext_polynomial
}  // namespace calculus
