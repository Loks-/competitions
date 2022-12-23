#pragma once

#include "common/numeric/fraction.h"
#include "common/numeric/long/signed.h"
#include "common/numeric/long/signed/gcd.h"
#include "common/numeric/long/signed/multiplication.h"

using TLFraction = numeric::Fraction<numeric::nlong::Signed>;

inline TLFraction MakeTLFraction(int numerator) {
  return TLFraction(numeric::nlong::Signed(numerator));
}

inline TLFraction MakeTLFraction(int64_t numerator) {
  return TLFraction(numeric::nlong::Signed(numerator));
}
