#pragma once

#include "common/numeric/factorial.h"

inline uint64_t GetCatalan(unsigned n) {
  return GetFactorial(2 * n) / GetFactorial(n) / GetFactorial(n + 1);
}
