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
    case GenerationType::PRIMALITY_TEST:
      vp = GetPrimes_PrimalityTest(maxn);
      break;
    case GenerationType::PRIMES_LIST:
      vp = GetPrimes_PrimesList(maxn);
      break;
  }
  size_t h = std::hash<std::vector<uint64_t>>()(vp);
  std::cout << name << ": " << h << "\t" << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterPrimeGeneration::TestAll(bool time_test) {
  uint64_t maxn = (time_test ? 10000000 : 1000000);
  std::unordered_set<size_t> hs;
  hs.insert(Test("PrimalityT", maxn, GenerationType::PRIMALITY_TEST));
  hs.insert(Test("PrimesList", maxn, GenerationType::PRIMES_LIST));
  return hs.size() == 1;
}

bool TestPrimeGeneration(bool time_test) {
  return TesterPrimeGeneration::TestAll(time_test);
}
