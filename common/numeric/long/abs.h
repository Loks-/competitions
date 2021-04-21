#pragma once

#include "common/numeric/long/signed.h"
#include "common/numeric/utils/abs.h"

template <>
inline LongSigned Abs<LongSigned>(const LongSigned& val) {
  return LongSigned(val.GetUnsigned());
}
