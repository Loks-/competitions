#include "tester/tester_primes_count.h"

#include "tester/primes_count.h"

#include "common/factorization/primes_generator.h"
#include "common/timer.h"

#include <iostream>
#include <string>
#include <unordered_set>

size_t TesterPrimesCount::Test(const std::string& name, uint64_t n,
                               Algorithm type, unsigned extra) {
  Timer t;
  uint64_t r = 0;
  switch (type) {
    case Algorithm::TABLE:
      r = PrimesCount_Table(n);
      break;
    case Algorithm::PRIMES_GENERATION:
      r = PrimesCount_PrimesGeneration(n);
      break;
    case Algorithm::LEGENDRE:
      r = PrimesCount_Legendre(n);
      break;
    case Algorithm::MEISSEL:
      r = PrimesCount_Meissel(n);
      break;
    case Algorithm::MEISSEL_LEHMER:
      r = extra ? PrimesCount_MeisselLehmerB(n, extra)
                : PrimesCount_MeisselLehmer1(n);
      break;
    case Algorithm::LUCY_HEDGEHOG_RECURSIVE:
      r = PrimesCount_LucyHedgehogRecursive(n);
      break;
    case Algorithm::LUCY_HEDGEHOG_RECURSIVE2:
      r = PrimesCount_LucyHedgehogRecursive2(n);
      break;
    case Algorithm::LUCY_HEDGEHOG_VECTOR:
      r = PrimesCount_LucyHedgehogVector(n);
      break;
    case Algorithm::PRIMES_COUNT_QUOTIENTS:
      r = PrimesCount_PCQ(n);
      break;
  }
  std::cout << name << ": " << r << "\t" << t.GetMilliseconds() << std::endl;
  return r;
}

bool TesterPrimesCount::TestAll(bool time_test) {
  uint64_t n = (time_test ? 10000000000ull : 1000000);
  std::unordered_set<size_t> rs;
  rs.insert(Test("Table           ", n, Algorithm::TABLE));
  if (!time_test) {
    rs.insert(Test("PrimesGeneration", n, Algorithm::PRIMES_GENERATION));
    rs.insert(Test("Legendre        ", n, Algorithm::LEGENDRE));
  }
  rs.insert(Test("Meissel         ", n, Algorithm::MEISSEL));
  rs.insert(Test("MeisselLehmer 0 ", n, Algorithm::MEISSEL_LEHMER, 0));
  rs.insert(Test("MeisselLehmer 1 ", n, Algorithm::MEISSEL_LEHMER, 1));
  rs.insert(Test("MeisselLehmer 2 ", n, Algorithm::MEISSEL_LEHMER, 2));
  rs.insert(Test("MeisselLehmer 3 ", n, Algorithm::MEISSEL_LEHMER, 3));
  rs.insert(Test("MeisselLehmer 4 ", n, Algorithm::MEISSEL_LEHMER, 4));
  rs.insert(Test("MeisselLehmer 5 ", n, Algorithm::MEISSEL_LEHMER, 5));
  rs.insert(Test("LucyHedgehogR   ", n, Algorithm::LUCY_HEDGEHOG_RECURSIVE));
  rs.insert(Test("LucyHedgehogR2  ", n, Algorithm::LUCY_HEDGEHOG_RECURSIVE2));
  rs.insert(Test("LucyHedgehogV   ", n, Algorithm::LUCY_HEDGEHOG_VECTOR));
  rs.insert(Test("PCQ             ", n, Algorithm::PRIMES_COUNT_QUOTIENTS));
  return rs.size() == 1;
}

bool TestPrimesCount(bool time_test) {
  return TesterPrimesCount::TestAll(time_test);
}
