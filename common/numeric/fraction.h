#pragma once

#include "common/base.h"
#include "common/numeric/utils/gcd.h"

namespace numeric {
template <class TTValue>
class Fraction {
 public:
  using TValue = TTValue;
  using TSelf = Fraction<TValue>;

 protected:
  TValue numerator, denominator;

 public:
  void Simplify() {
    if (denominator < 0) {
      numerator = -numerator;
      denominator = -denominator;
    }
    TValue g =
        GCD<TValue>((numerator < 0 ? -numerator : numerator), denominator);
    numerator /= g;
    denominator /= g;
  }

  Fraction() : numerator(0), denominator(1) {}

  explicit Fraction(const TValue& value) : numerator(value), denominator(1) {}

  Fraction(const TValue& _numerator, const TValue& _denominator)
      : numerator(_numerator), denominator(_denominator) {
    assert(denominator != TValue(0));
    Simplify();
  }

  const TValue& GetN() const { return numerator; }
  const TValue& GetD() const { return denominator; }

  bool operator==(const TSelf& r) const {
    return (numerator == r.numerator) && (denominator == r.denominator);
  }

  bool operator!=(const TSelf& r) const {
    return (numerator != r.numerator) || (denominator != r.denominator);
  }

  bool operator<(const TSelf& r) const {
    return numerator * r.denominator < r.numerator * denominator;
  }

  bool operator<=(const TSelf& r) const {
    return numerator * r.denominator <= r.numerator * denominator;
  }

  bool operator>(const TSelf& r) const {
    return numerator * r.denominator > r.numerator * denominator;
  }

  bool operator>=(const TSelf& r) const {
    return numerator * r.denominator >= r.numerator * denominator;
  }

  bool operator==(const TValue& r) const {
    return (numerator == r) && (denominator == 1);
  }

  bool operator!=(const TValue& r) const {
    return (numerator != r) || (denominator != 1);
  }

  bool operator<(const TValue& r) const { return numerator < r * denominator; }

  bool operator<=(const TValue& r) const {
    return numerator <= r * denominator;
  }

  bool operator>(const TValue& r) const { return numerator > r * denominator; }

  bool operator>=(const TValue& r) const {
    return numerator >= r * denominator;
  }

  TSelf operator*(const TValue& value) const {
    return TSelf(numerator * value, denominator);
  }

  TSelf operator/(const TValue& value) const {
    assert(value != TValue(0));
    return TSelf(numerator, denominator * value);
  }

  TSelf& operator*=(const TValue& value) {
    numerator *= value;
    Simplify();
    return *this;
  }

  TSelf& operator/=(const TValue& value) {
    assert(value != TValue(0));
    denominator *= value;
    Simplify();
    return *this;
  }

  TSelf operator*(const TSelf& value) const {
    return TSelf(numerator * value.numerator, denominator * value.denominator);
  }

  TSelf operator/(const TSelf& value) const {
    assert(value.numerator != TValue(0));
    return TSelf(numerator * value.denominator, denominator * value.numerator);
  }

  TSelf& operator*=(const TSelf& value) {
    numerator *= value.numerator;
    denominator *= value.denominator;
    Simplify();
    return *this;
  }

  TSelf& operator/=(const TSelf& value) {
    assert(value.numerator != TValue(0));
    numerator *= value.denominator;
    denominator *= value.numerator;
    Simplify();
    return *this;
  }

  TSelf operator+(const TSelf& value) const {
    return TSelf(numerator * value.denominator + denominator * value.numerator,
                 denominator * value.denominator);
  }

  TSelf operator-(const TSelf& value) const {
    return TSelf(numerator * value.denominator - denominator * value.numerator,
                 denominator * value.denominator);
  }

  TSelf& operator+=(const TSelf& value) {
    numerator = numerator * value.denominator + denominator * value.numerator;
    denominator *= value.denominator;
    Simplify();
    return *this;
  }

  TSelf& operator-=(const TSelf& value) {
    numerator = numerator * value.denominator - denominator * value.numerator;
    denominator *= value.denominator;
    Simplify();
    return *this;
  }
};
}  // namespace numeric

using TIFraction = numeric::Fraction<int64_t>;
