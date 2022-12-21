#pragma once

#include "common/numeric/fraction.h"
#include "common/numeric/long/signed.h"
#include "common/numeric/long/signed/gcd.h"
#include "common/numeric/long/signed/multiplication.h"

using TLFraction = numeric::Fraction<numeric::nlong::Signed>;

TLFraction MakeTLFraction(int x) {
  return TLFraction(numeric::nlong::Signed(x));
}

TLFraction MakeTLFraction(int64_t x) {
  return TLFraction(numeric::nlong::Signed(x));
}
