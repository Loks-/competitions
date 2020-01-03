#pragma once

#include "common/base.h"
#include <vector>

std::vector<uint64_t> GetPrimes_TrialDivision(uint64_t maxn);
std::vector<uint64_t> GetPrimes_PrimalityTest(uint64_t maxn);
std::vector<uint64_t> GetPrimes_PrimesTable(uint64_t maxn);
std::vector<uint64_t> GetPrimes_EratosthenesBit(uint64_t maxn);
std::vector<uint64_t> GetPrimes_EratosthenesByte(uint64_t maxn);
std::vector<uint64_t> GetPrimes_EratosthenesInt(uint64_t maxn);
std::vector<uint64_t> GetPrimes_EratosthenesOdd(uint64_t maxn);
std::vector<uint64_t> GetPrimes_EratosthenesOddMemoryReduced(uint64_t maxn);
std::vector<uint64_t> GetPrimes_EratosthenesSegmented(uint64_t maxn);
std::vector<uint64_t> GetPrimes_AtkinBit(uint64_t maxn);
std::vector<uint64_t> GetPrimes_AtkinByte(uint64_t maxn);
std::vector<uint64_t> GetPrimes_AtkinInt(uint64_t maxn);
std::vector<uint64_t> GetPrimes_AtkinMemoryReduced(uint64_t maxn);
