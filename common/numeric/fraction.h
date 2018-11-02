#pragma once

#include "../base.h"
template <class TValue>
class Fraction
{
protected:
    TValue numerator, denominator;

public:
    Fraction() : numerator(0), denominator(1) {}
    Fraction(const TValue& value) : numerator(value), denominator(1) {}
    Fraction(const TValue& _numerator, const TValue& _denominator) : numerator(_numerator), denominator(_denominator) {}

    const TValue& GetN() const { return numerator; }
    const TValue& GetD() const { return denominator; }
};

using TIFraction = Fraction<int64_t>;
