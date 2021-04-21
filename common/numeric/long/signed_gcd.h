#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/long/unsigned_gcd.h"
#include "common/numeric/utils/gcd.h"

template <>
inline LongSigned GCD<LongSigned>(LongSigned a, LongSigned b) {
  return LongSigned(GCD(a.GetUnsigned(), b.GetUnsigned()));
}

template <>
inline LongSigned GCDSafe<LongSigned>(LongSigned a, LongSigned b) {
  return LongSigned(GCD(a.GetUnsigned(), b.GetUnsigned()));
}
