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
  virtual ~TermBase() {}

  TValue TermBaseGet(const TValue& x) const = 0;
  bool TermBaseLess(const TermBase& r) const { return false; }
  bool TermBaseEqual(const TermBase& r) const = 0;

  TValue Get(const TValue& x) const { return TermBaseGet(x) * PowS(x, power); }

  bool operator<(const TermBase& r) const {
    return (power < r.power) ? true : (power > r.power) ? false
                                                        : TermBaseLess(r);
  }

  bool operator==(const TermBase& r) const {
    return (power == r.power) ? TermBaseEqual(r) : false;
  }
};

template <class TValue>
using PTermBase = std::shared_ptr<TermBase<TValue>>;
}  // namespace ext_polynomial
}  // namespace calculus
