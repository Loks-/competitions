#pragma once

#include "common/base.h"
#include <string>

class TesterPrimeGeneration {
 public:
  enum class GenerationType {
    TRIAL_DIVISION,
    PRIMALITY_TEST,
    PRIMES_TABLE,
    ERATOSTHENES_BIT,
    ERATOSTHENES_BYTE,
    ERATOSTHENES_INT,
    ERATOSTHENES_ODD,
    ERATOSTHENES_OMR,
    ERATOSTHENES_SS,
    ATKIN_BIT,
    ATKIN_BYTE,
    ATKIN_INT,
    ATKIN_MR
  };

 public:
  static size_t Test(const std::string& name, uint64_t maxn,
                     GenerationType type);
  static size_t TestPG(uint64_t maxn, uint64_t block_size);
  static bool TestAll(bool time_test);
};
