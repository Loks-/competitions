#include "tester/tester_primes_count.h"

#include "tester/primes_count.h"
#include "common/factorization/primes_generator.h"
#include "common/timer.h"
#include <iostream>
#include <string>
#include <unordered_set>

size_t TesterPrimesCount::Test(const std::string& name, uint64_t n,
                               Algorithm type) {
  Timer t;
  uint64_t r = 0;
  switch (type) {
    case Algorithm::TABLE:
      r = PrimesCount_Table(n);
      break;
    case Algorithm::PRIMES_GENERATION:
      r = PrimesCount_PrimesGeneration(n);
      break;
  }
  std::cout << name << ": " << r << "\t" << t.GetMilliseconds() << std::endl;
  return r;
}

bool TesterPrimesCount::TestAll(bool time_test) {
  uint64_t n = (time_test ? 100000000ull : 1000000);
  std::unordered_set<size_t> rs;
  rs.insert(Test("Table           ", n, Algorithm::TABLE));
  rs.insert(Test("PrimesGeneration", n, Algorithm::PRIMES_GENERATION));
  return rs.size() == 1;
}

bool TestPrimesCount(bool time_test) {
  return TesterPrimesCount::TestAll(time_test);
}
