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
  constexpr void Simplify() {
    if (denominator < 0) {
      numerator = -numerator;
      denominator = -denominator;
    }
    TValue g = GCDSafe(numerator, denominator);
    numerator /= g;
    denominator /= g;
  }

  constexpr Fraction() : numerator(0), denominator(1) {}

  constexpr explicit Fraction(const TValue& value)
      : numerator(value), denominator(1) {}

  constexpr Fraction(const TValue& _numerator, const TValue& _denominator)
      : numerator(_numerator), denominator(_denominator) {
    assert(denominator != TValue(0));
    Simplify();
  }

  constexpr const TValue& GetN() const { return numerator; }
  constexpr const TValue& GetD() const { return denominator; }

  constexpr double ToDouble() const {
    return double(numerator) / double(denominator);
  }

  constexpr TSelf operator-() const { return TSelf(-numerator, denominator); }

  constexpr bool operator==(const TSelf& r) const {
    return (numerator == r.numerator) && (denominator == r.denominator);
  }

  constexpr bool operator!=(const TSelf& r) const {
    return (numerator != r.numerator) || (denominator != r.denominator);
  }

  constexpr bool operator<(const TSelf& r) const {
    return numerator * r.denominator < r.numerator * denominator;
  }

  constexpr bool operator<=(const TSelf& r) const {
    return numerator * r.denominator <= r.numerator * denominator;
  }

  constexpr bool operator>(const TSelf& r) const {
    return numerator * r.denominator > r.numerator * denominator;
  }

  constexpr bool operator>=(const TSelf& r) const {
    return numerator * r.denominator >= r.numerator * denominator;
  }

  constexpr bool operator==(const TValue& r) const {
    return (numerator == r) && (denominator == 1);
  }

  constexpr bool operator!=(const TValue& r) const {
    return (numerator != r) || (denominator != 1);
  }

  constexpr bool operator<(const TValue& r) const {
    return numerator < r * denominator;
  }

  constexpr bool operator<=(const TValue& r) const {
    return numerator <= r * denominator;
  }

  constexpr bool operator>(const TValue& r) const {
    return numerator > r * denominator;
  }

  constexpr bool operator>=(const TValue& r) const {
    return numerator >= r * denominator;
  }

  constexpr TSelf operator*(const TValue& value) const {
    return TSelf(numerator * value, denominator);
  }

  constexpr TSelf operator/(const TValue& value) const {
    assert(value != TValue(0));
    return TSelf(numerator, denominator * value);
  }

  constexpr TSelf& operator*=(const TValue& value) {
    numerator *= value;
    Simplify();
    return *this;
  }

  constexpr TSelf& operator/=(const TValue& value) {
    assert(value != TValue(0));
    denominator *= value;
    Simplify();
    return *this;
  }

  constexpr TSelf operator*(const TSelf& value) const {
    return TSelf(numerator * value.numerator, denominator * value.denominator);
  }

  constexpr TSelf operator/(const TSelf& value) const {
    assert(value.numerator != TValue(0));
    return TSelf(numerator * value.denominator, denominator * value.numerator);
  }

  constexpr TSelf& operator*=(const TSelf& value) {
    numerator *= value.numerator;
    denominator *= value.denominator;
    Simplify();
    return *this;
  }

  constexpr TSelf& operator/=(const TSelf& value) {
    assert(value.numerator != TValue(0));
    numerator *= value.denominator;
    denominator *= value.numerator;
    Simplify();
    return *this;
  }

  constexpr TSelf operator+(const TSelf& value) const {
    return TSelf(numerator * value.denominator + denominator * value.numerator,
                 denominator * value.denominator);
  }

  constexpr TSelf operator-(const TSelf& value) const {
    return TSelf(numerator * value.denominator - denominator * value.numerator,
                 denominator * value.denominator);
  }

  constexpr TSelf& operator+=(const TSelf& value) {
    numerator = numerator * value.denominator + denominator * value.numerator;
    denominator *= value.denominator;
    Simplify();
    return *this;
  }

  constexpr TSelf& operator-=(const TSelf& value) {
    numerator = numerator * value.denominator - denominator * value.numerator;
    denominator *= value.denominator;
    Simplify();
    return *this;
  }
};
}  // namespace numeric

using TIFraction = numeric::Fraction<int64_t>;
