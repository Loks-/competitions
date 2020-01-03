#pragma once

#include "common/base.h"
#include <string>

class TesterPrimesCount {
 public:
  enum class Algorithm { TABLE, PRIMES_GENERATION, LEGENDRE };

 public:
  static size_t Test(const std::string& name, uint64_t n, Algorithm type);
  static bool TestAll(bool time_test);
};
