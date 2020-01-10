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
    LUCY_HEDGEHOG_RECURSIVE,
    LUCY_HEDGEHOG_RECURSIVE2,
    LUCY_HEDGEHOG_VECTOR,
    PRIMES_COUNT_QUOTIENTS
  };

 public:
  static size_t Test(const std::string& name, uint64_t n, Algorithm type);
  static bool TestAll(bool time_test);
};
