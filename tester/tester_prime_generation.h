#pragma once

#include "common/base.h"
#include <string>

class TesterPrimeGeneration {
 public:
  enum class GenerationType {
    PRIMALITY_TEST,
    PRIMES_LIST,
    ERATOSTHENES_BIT,
    ERATOSTHENES_BYTE,
    ERATOSTHENES_INT,
    ERATOSTHENES_ODD,
    ERATOSTHENES_OMR
  };

 public:
  static size_t Test(const std::string& name, uint64_t maxn,
                     GenerationType type);
  static bool TestAll(bool time_test);
};
