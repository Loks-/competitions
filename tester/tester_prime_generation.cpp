#include "tester/tester_prime_generation.h"

#include "tester/prime_generation.h"
#include "common/factorization/primes_generator.h"
#include "common/stl/hash/vector.h"
#include "common/timer.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

size_t TesterPrimeGeneration::Test(const std::string& name, uint64_t maxn,
                                   Algorithm type) {
  Timer t;
  std::vector<uint64_t> vp;
  switch (type) {
    case Algorithm::TRIAL_DIVISION:
      vp = GetPrimes_TrialDivision(maxn);
      break;
    case Algorithm::PRIMALITY_TEST:
      vp = GetPrimes_PrimalityTest(maxn);
      break;
    case Algorithm::PRIMES_TABLE:
      vp = GetPrimes_PrimesTable(maxn);
      break;
    case Algorithm::ERATOSTHENES_BIT:
      vp = GetPrimes_EratosthenesBit(maxn);
      break;
    case Algorithm::ERATOSTHENES_BYTE:
      vp = GetPrimes_EratosthenesByte(maxn);
      break;
    case Algorithm::ERATOSTHENES_INT:
      vp = GetPrimes_EratosthenesInt(maxn);
      break;
    case Algorithm::ERATOSTHENES_ODD:
      vp = GetPrimes_EratosthenesOdd(maxn);
      break;
    case Algorithm::ERATOSTHENES_OMR:
      vp = GetPrimes_EratosthenesOddMemoryReduced(maxn);
      break;
    case Algorithm::ERATOSTHENES_SS:
      vp = GetPrimes_EratosthenesSegmented(maxn);
      break;
    case Algorithm::ATKIN_BIT:
      vp = GetPrimes_AtkinBit(maxn);
      break;
    case Algorithm::ATKIN_BYTE:
      vp = GetPrimes_AtkinByte(maxn);
      break;
    case Algorithm::ATKIN_INT:
      vp = GetPrimes_AtkinInt(maxn);
      break;
    case Algorithm::ATKIN_MR:
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
  std::vector<uint64_t> vprimes;
  for (uint64_t p = pg.Get(); p <= maxn; p = pg.GetNext()) vprimes.push_back(p);
  size_t h = std::hash<std::vector<uint64_t>>()(vprimes);
  std::string name_suffix = std::to_string(block_size);
  std::cout << "PG" << std::string(14 - name_suffix.size(), ' ') << name_suffix
            << ": " << h << "\t" << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterPrimeGeneration::TestAll(bool time_test) {
  uint64_t maxn = (time_test ? 100000000ull : 1000000);
  std::unordered_set<size_t> hs;
  if (!time_test) {
    hs.insert(Test("TrialDivision   ", maxn, Algorithm::TRIAL_DIVISION));
    hs.insert(Test("PrimalityTest   ", maxn, Algorithm::PRIMALITY_TEST));
  }
  hs.insert(Test("PrimesTable     ", maxn, Algorithm::PRIMES_TABLE));
  hs.insert(Test("EratosthenesBit ", maxn, Algorithm::ERATOSTHENES_BIT));
  hs.insert(Test("EratosthenesByte", maxn, Algorithm::ERATOSTHENES_BYTE));
  hs.insert(Test("EratosthenesInt ", maxn, Algorithm::ERATOSTHENES_INT));
  hs.insert(Test("EratosthenesOdd ", maxn, Algorithm::ERATOSTHENES_ODD));
  hs.insert(Test("EratosthenesOMR ", maxn, Algorithm::ERATOSTHENES_OMR));
  hs.insert(Test("EratosthenesSS  ", maxn, Algorithm::ERATOSTHENES_SS));
  hs.insert(Test("AtkinBit        ", maxn, Algorithm::ATKIN_BIT));
  hs.insert(Test("AtkinByte       ", maxn, Algorithm::ATKIN_BYTE));
  hs.insert(Test("AtkinInt        ", maxn, Algorithm::ATKIN_INT));
  hs.insert(Test("AtkinMR         ", maxn, Algorithm::ATKIN_MR));
  {
    unsigned block_size = (time_test ? 65536 : 4096);
    for (unsigned i = 0; i < 5; ++i) {
      block_size *= 2;
      hs.insert(TestPG(maxn, block_size));
    }
  }
  return hs.size() == 1;
}

bool TestPrimeGeneration(bool time_test) {
  return TesterPrimeGeneration::TestAll(time_test);
}
