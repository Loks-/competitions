#pragma once

#include "common/numeric/long/unsigned.h"

inline LongUnsigned GetFactorialL(unsigned n) {
  LongUnsigned l(1u);
  for (unsigned i = 2; i <= n; ++i) l *= i;
  return l;
}
