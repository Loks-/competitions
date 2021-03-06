#pragma once

#include "common/base.h"

#include <string>

class TesterPrimesCount {
 public:
  enum class Algorithm {
    TABLE,
    PRIMES_GENERATION,
    LEGENDRE,
    MEISSEL,
    MEISSEL_LEHMER,
    LAGARIAS_MILLER_ODLYZKO,
    LAGARIAS_MILLER_ODLYZKO_Y,
    DELEGLISE_RIVAT,
    LUCY_HEDGEHOG_RECURSIVE,
    LUCY_HEDGEHOG_RECURSIVE2,
    LUCY_HEDGEHOG_VECTOR,
    PRIMES_COUNT,
    PRIMES_COUNT_QUOTIENTS
  };

 public:
  static size_t Test(const std::string& name, uint64_t n, Algorithm type,
                     unsigned extra = 0);
  static bool TestAll(bool time_test);
};
