#include "common/numeric/long/unsigned/multiplication.h"
#include "common/numeric/long/unsigned_io.h"
#include "common/numeric/long/utils/factorial.h"

#include <iostream>

bool TestLongMult() {
  unsigned n = 10000, l = n / 3, k = 1;
  LongUnsigned a(1u), b(1u);
  for (; k < l;) a *= ++k;
  for (; k < n;) b *= ++k;
  LongUnsigned r = GetFactorialL(n), r1 = numeric::nlong::MultBase(a, b),
               r2 = numeric::nlong::MultFFT(a, b);
  if ((r1 != r) || (r2 != r)) {
    std::cout << n << "\n"
              << a << "\n"
              << b << "\n"
              << r << "\n"
              << r1 << "\n"
              << r2 << std::endl;
    return false;
  }
  return true;
}
