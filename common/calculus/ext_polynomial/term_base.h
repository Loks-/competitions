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

  virtual TValue TermBaseGet(const TValue& x) const = 0;
  virtual bool TermBaseLess(const TermBase& r) const { return false; }
  virtual bool TermBaseEqual(const TermBase& r) const = 0;

  virtual TValue Get(const TValue& x) const {
    return TermBaseGet(x) * PowS(x, power);
  }

  virtual bool operator<(const TermBase& r) const {
    return (power < r.power) ? true : (power > r.power) ? false
                                                        : TermBaseLess(r);
  }

  virtual bool operator==(const TermBase& r) const {
    return (power == r.power) ? TermBaseEqual(r) : false;
  }
};

template <class TValue>
using PTermBase = std::shared_ptr<TermBase<TValue>>;
}  // namespace ext_polynomial
}  // namespace calculus
