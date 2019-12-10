#include "tester/tester_prime_generation.h"

#include "tester/prime_generation.h"
#include "common/factorization/primes.h"
#include "common/stl/hash/vector.h"
#include "common/timer.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

size_t TesterPrimeGeneration::Test(const std::string& name, uint64_t maxn,
                                   GenerationType type) {
  Timer t;
  std::vector<uint64_t> vp;
  switch (type) {
    case GenerationType::TRIAL_DIVISION:
      vp = GetPrimes_TrialDivision(maxn);
      break;
    case GenerationType::PRIMALITY_TEST:
      vp = GetPrimes_PrimalityTest(maxn);
      break;
    case GenerationType::PRIMES_LIST:
      vp = GetPrimes_PrimesList(maxn);
      break;
    case GenerationType::ERATOSTHENES_BIT:
      vp = GetPrimes_EratosthenesBit(maxn);
      break;
    case GenerationType::ERATOSTHENES_BYTE:
      vp = GetPrimes_EratosthenesByte(maxn);
      break;
    case GenerationType::ERATOSTHENES_INT:
      vp = GetPrimes_EratosthenesInt(maxn);
      break;
    case GenerationType::ERATOSTHENES_ODD:
      vp = GetPrimes_EratosthenesOdd(maxn);
      break;
    case GenerationType::ERATOSTHENES_OMR:
      vp = GetPrimes_EratosthenesOddMemoryReduced(maxn);
      break;
    case GenerationType::ERATOSTHENES_SS:
      vp = GetPrimes_EratosthenesSegmented(maxn);
      break;
    case GenerationType::ATKIN_BIT:
      vp = GetPrimes_AtkinBit(maxn);
      break;
    case GenerationType::ATKIN_BYTE:
      vp = GetPrimes_AtkinByte(maxn);
      break;
    case GenerationType::ATKIN_INT:
      vp = GetPrimes_AtkinInt(maxn);
      break;
    case GenerationType::ATKIN_MR:
      vp = GetPrimes_AtkinMemoryReduced(maxn);
      break;
  }
  size_t h = std::hash<std::vector<uint64_t>>()(vp);
  std::cout << name << ": " << h << "\t" << t.GetMilliseconds() << std::endl;
  return h;
}

size_t TesterPrimeGeneration::TestPG(uint64_t maxn, uint64_t block_size) {
  Timer t;
  factorization::PrimesGenerator pg(block_size);
  size_t h = std::hash<std::vector<uint64_t>>()(pg.GeneratePrimes(maxn));
  std::string name_suffix = std::to_string(block_size);
  std::cout << "PG" << std::string(14 - name_suffix.size(), ' ') << name_suffix
            << ": " << h << "\t" << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterPrimeGeneration::TestAll(bool time_test) {
  uint64_t maxn = (time_test ? 100000000ull : 1000000);
  std::unordered_set<size_t> hs;
  if (!time_test) {
    hs.insert(Test("TrialDivision   ", maxn, GenerationType::TRIAL_DIVISION));
    hs.insert(Test("PrimalityTest   ", maxn, GenerationType::PRIMALITY_TEST));
  }
  hs.insert(Test("PrimesList      ", maxn, GenerationType::PRIMES_LIST));
  hs.insert(Test("EratosthenesBit ", maxn, GenerationType::ERATOSTHENES_BIT));
  hs.insert(Test("EratosthenesByte", maxn, GenerationType::ERATOSTHENES_BYTE));
  hs.insert(Test("EratosthenesInt ", maxn, GenerationType::ERATOSTHENES_INT));
  hs.insert(Test("EratosthenesOdd ", maxn, GenerationType::ERATOSTHENES_ODD));
  hs.insert(Test("EratosthenesOMR ", maxn, GenerationType::ERATOSTHENES_OMR));
  hs.insert(Test("EratosthenesSS  ", maxn, GenerationType::ERATOSTHENES_SS));
  hs.insert(Test("AtkinBit        ", maxn, GenerationType::ATKIN_BIT));
  hs.insert(Test("AtkinByte       ", maxn, GenerationType::ATKIN_BYTE));
  hs.insert(Test("AtkinInt        ", maxn, GenerationType::ATKIN_INT));
  hs.insert(Test("AtkinMR         ", maxn, GenerationType::ATKIN_MR));
  {
    unsigned block_size = 4096;
    for (unsigned i = 0; i < (time_test ? 10 : 5); ++i) {
      block_size *= 2;
      TestPG(maxn, block_size);
    }
  }
  return hs.size() == 1;
}

bool TestPrimeGeneration(bool time_test) {
  return TesterPrimeGeneration::TestAll(time_test);
}
