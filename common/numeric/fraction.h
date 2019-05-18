#pragma once

#include "common/base.h"
#include "common/numeric/utils/gcd.h"

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
  Fraction(const TValue& value) : numerator(value), denominator(1) {}
  Fraction(const TValue& _numerator, const TValue& _denominator)
      : numerator(_numerator), denominator(_denominator) {
    Simplify();
  }

  const TValue& GetN() const { return numerator; }
  const TValue& GetD() const { return denominator; }

  bool operator==(const TSelf& r) const {
    return (numerator == r.numerator) && (denominator == r.denominator);
  }

  bool operator<(const TSelf& r) const {
    return numerator * r.denominator < r.numerator * denominator;
  }
};

using TIFraction = Fraction<int64_t>;
