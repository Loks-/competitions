#pragma once

#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/continued_fraction/convergent.h"
#include "common/numeric/fraction.h"

namespace numeric {
namespace cf {
class CFSemiConvergent : public CFConvergent {
 public:
  using TBase = CFConvergent;
  using TCFVector = ContinuedFraction::TCFVector;

 protected:
  int64_t its;

 protected:
  constexpr int64_t ItsLast() const {
    return End() ? 1 : TBase::cf(TBase::itc);
  }

  constexpr void AdjustSemiIterator() {
    its = ItsLast() / 2;
    if ((ItsLast() & 1) || !IsHalfValid()) ++its;
  }

  constexpr bool IsHalfValid() const {
    const std::vector<int64_t>& v = TBase::cf.GetVector();
    const ContinuedFraction cfl(TCFVector(v.begin() + TBase::itc, v.end()));
    const ContinuedFraction cfr(
        TCFVector(v.rend() - TBase::itc - 1, v.rend() - 1));
    return cfl < cfr;
  }

 public:
  constexpr void ResetIterator() {
    TBase::ResetIteratorI();
    its = ItsLast();
  }

  constexpr explicit CFSemiConvergent(
      const ContinuedFraction& continued_fraction)
      : TBase(continued_fraction) {
    ResetIterator();
  }

  constexpr bool End() const { return TBase::itc >= TBase::cf.Size(); }

  constexpr TIFraction Get() const {
    return TIFraction(its * TBase::n0 + TBase::n1, its * TBase::d0 + TBase::d1);
  }

  constexpr CFSemiConvergent& Next() {
    if (its == ItsLast()) {
      TBase::Next();
      AdjustSemiIterator();
    } else
      ++its;
    return *this;
  }

  constexpr TIFraction operator*() const { return Get(); }

  constexpr CFSemiConvergent& operator++() { return Next(); }
};
}  // namespace cf
}  // namespace numeric
