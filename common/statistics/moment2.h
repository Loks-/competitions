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
  Moment2() { Clear(); }

  void Clear() {
    TBase::Clear();
    s2 = TValue(0);
  }

  void AddSample(const TValue& x, const TValue& w = 1) {
    TBase::AddSample(x, w);
    s2 += x * x * w;
  }

  void MultWeight(const TValue& x) {
    TBase::MultWeight(x);
    s2 *= x;
  }

  void AddValue(const TValue& x) {
    s2 += x * x * TBase::s0 + 2 * x * TBase::s1;
    TBase::AddValue(x);
  }

  void AddStat(const TSelf& r) {
    TBase::AddStat(r);
    s2 += r.s2;
  }

  TSelf& operator+=(const TValue& x) {
    AddValue(x);
    return *this;
  }

  TSelf& operator-=(const TValue& x) {
    AddValue(-x);
    return *this;
  }

  TSelf& operator*=(const TValue& x) {
    MultWeight(x);
    return *this;
  }

  TSelf& operator+=(const TSelf& r) {
    AddStat(r);
    return *this;
  }

  TSelf operator+(const TValue& x) {
    TSelf t(*this);
    t += x;
    return t;
  }

  TSelf operator-(const TValue& x) {
    TSelf t(*this);
    t -= x;
    return t;
  }

  TSelf operator*(const TValue& x) {
    TSelf t(*this);
    t *= x;
    return t;
  }

  TSelf operator+(const TSelf& r) {
    TSelf t(*this);
    t += r;
    return t;
  }

  TValue RawMoment2() const { return s2 / TBase::s0; }

  TValue CentralMoment2() const {
    TValue m = TBase::RawMoment1();
    return RawMoment2() - m * m;
  }

  TValue Variance() const { return CentralMoment2(); }
};
}  // namespace stat
