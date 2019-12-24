#include "common/base.h"
#include "common/factorization/table/mobius.h"
#include "common/numeric/utils/usqrt.h"

inline uint64_t SquareFreeCount(const factorization::table::Mobius& mt,
                                uint64_t n) {
  uint64_t nsqrt = USqrt(n);
  assert(nsqrt <= mt.GetTableSize());
  int64_t s = 0;
  for (uint64_t i = 1; i <= nsqrt; ++i) s += mt(i) * int64_t(n / i / i);
  return s;
}
