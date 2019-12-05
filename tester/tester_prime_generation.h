#pragma once

#include "common/base.h"
#include <string>

class TesterPrimeGeneration {
 public:
  enum class GenerationType { PRIMALITY_TEST, PRIMES_LIST, END };

 public:
  static size_t Test(const std::string& name, uint64_t maxn,
                     GenerationType type);
  static bool TestAll(bool time_test);
};
