#pragma once

namespace stat {
template <class TValue = double>
class Moment1 {
 public:
  using TSelf = Moment1<TValue>;

 protected:
  TValue s0, s1;

 public:
  constexpr Moment1() { Clear(); }

  constexpr void Clear() { s0 = s1 = TValue(0); }

  constexpr void AddSample(const TValue& x, const TValue& w = 1) {
    s0 += w;
    s1 += x * w;
  }

  constexpr void AddValue(const TValue& x) { s1 += x * s0; }

  constexpr void MultWeight(const TValue& x) {
    s0 *= x;
    s1 *= x;
  }

  constexpr void AddStat(const TSelf& r) {
    s0 += r.s0;
    s1 += r.s1;
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

  constexpr TValue RawMoment1() const { return s1 / s0; }

  constexpr TValue Mean() const { return RawMoment1(); }
};
}  // namespace stat
