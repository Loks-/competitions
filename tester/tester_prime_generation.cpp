#include "tester/tester_prime_generation.h"

#include "tester/prime_generation.h"
#include "common/stl/hash/vector.h"
#include "common/timer.h"
#include <iostream>
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
  }
  size_t h = std::hash<std::vector<uint64_t>>()(vp);
  std::cout << name << ": " << h << "\t" << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterPrimeGeneration::TestAll(bool time_test) {
  uint64_t maxn = (time_test ? 100000000 : 1000000);
  std::unordered_set<size_t> hs;
  if (!time_test)
    hs.insert(Test("TrialDivision   ", maxn, GenerationType::TRIAL_DIVISION));
  if (!time_test)
    hs.insert(Test("PrimalityTest   ", maxn, GenerationType::PRIMALITY_TEST));
  hs.insert(Test("PrimesList      ", maxn, GenerationType::PRIMES_LIST));
  hs.insert(Test("EratosthenesBit ", maxn, GenerationType::ERATOSTHENES_BIT));
  hs.insert(Test("EratosthenesByte", maxn, GenerationType::ERATOSTHENES_BYTE));
  hs.insert(Test("EratosthenesInt ", maxn, GenerationType::ERATOSTHENES_INT));
  hs.insert(Test("EratosthenesOdd ", maxn, GenerationType::ERATOSTHENES_ODD));
  hs.insert(Test("EratosthenesOMR ", maxn, GenerationType::ERATOSTHENES_OMR));
  return hs.size() == 1;
}

bool TestPrimeGeneration(bool time_test) {
  return TesterPrimeGeneration::TestAll(time_test);
}
