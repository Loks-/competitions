#include "tester/primes_count.h"

#include "common/assert_exception.h"
#include "common/factorization/primes_range.h"
#include <vector>

uint64_t PrimesCount_Table(uint64_t n) {
  const std::vector<uint64_t> table{
      0,      4,       25,       168,       1229,          9592,          78498,
      664579, 5761455, 50847534, 455052511, 4118054813ull, 37607912018ull};
  uint64_t n10 = 1;
  for (unsigned i = 0; i < table.size(); ++i) {
    if (n == n10) return table[i];
    n10 *= 10;
  }
  Assert(false, "Unknown n for table");
  return 0;
}

uint64_t PrimesCount_PrimesGeneration(uint64_t n) {
  uint64_t c = 0;
  for (uint64_t p : factorization::PrimesRange(n)) ++c;
  return c;
}
