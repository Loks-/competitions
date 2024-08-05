#pragma once

#include "common/statistics/moment1.h"

namespace stat {
template <class TValue = double>
class Moment2 : public Moment1<TValue> {
 public:
  using TBase = Moment1<TValue>;
  using TSelf = Moment2<TValue>;

 protected:
  TValue s2;

 public:
  constexpr Moment2() { Clear(); }

  constexpr void Clear() {
    TBase::Clear();
    s2 = TValue(0);
  }

  constexpr void AddSample(const TValue& x, const TValue& w = 1) {
    TBase::AddSample(x, w);
    s2 += x * x * w;
  }

  constexpr void MultWeight(const TValue& x) {
    TBase::MultWeight(x);
    s2 *= x;
  }

  constexpr void AddValue(const TValue& x) {
    s2 += x * x * TBase::s0 + 2 * x * TBase::s1;
    TBase::AddValue(x);
  }

  constexpr void AddStat(const TSelf& r) {
    TBase::AddStat(r);
    s2 += r.s2;
  }

  constexpr TSelf& operator+=(const TValue& x) {
    AddValue(x);
    return *this;
  }

  constexpr TSelf& operator-=(const TValue& x) {
    AddValue(-x);
    return *this;
  }

  constexpr TSelf& operator*=(const TValue& x) {
    MultWeight(x);
    return *this;
  }

  constexpr TSelf& operator+=(const TSelf& r) {
    AddStat(r);
    return *this;
  }

  constexpr TSelf operator+(const TValue& x) {
    TSelf t(*this);
    t += x;
    return t;
  }

  constexpr TSelf operator-(const TValue& x) {
    TSelf t(*this);
    t -= x;
    return t;
  }

  constexpr TSelf operator*(const TValue& x) {
    TSelf t(*this);
    t *= x;
    return t;
  }

  constexpr TSelf operator+(const TSelf& r) {
    TSelf t(*this);
    t += r;
    return t;
  }

  constexpr TValue RawMoment2() const { return s2 / TBase::s0; }

  constexpr TValue CentralMoment2() const {
    TValue m = TBase::RawMoment1();
    return RawMoment2() - m * m;
  }

  constexpr TValue Variance() const { return CentralMoment2(); }
};
}  // namespace stat
