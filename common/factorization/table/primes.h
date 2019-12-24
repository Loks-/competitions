#pragma once

#include "common/base.h"
#include <vector>

namespace factorization {
namespace table {
class Primes {
 protected:
  uint64_t table_size, squared_table_size;
  std::vector<uint64_t> primes, squared_primes;
  std::vector<unsigned> table;

 public:
  Primes(uint64_t size) {
    table_size = size;
    squared_table_size = table_size * table_size;
    table.resize(table_size + 1, 0);
    table[0] = table[1] = 1;
    primes.push_back(2);
    for (uint64_t i = 2; i <= table_size; i += 2) table[i] = 2;
    for (uint64_t i = 3; i <= table_size; i += 2) {
      if (!table[i]) {
        primes.push_back(i);
        table[i] = unsigned(i);
        for (uint64_t j = i * i; j <= table_size; j += 2 * i) {
          if (table[j] == 0) table[j] = unsigned(i);
        }
      }
    }
    squared_primes.reserve(primes.size());
    for (uint64_t p : primes) squared_primes.push_back(p * p);
  }

  const std::vector<uint64_t>& GetPrimes() const { return primes; }

  const std::vector<uint64_t>& GetSquaredPrimes() const {
    return squared_primes;
  }

  uint64_t GetTableSize() const { return table_size; }
  uint64_t GetSquaredTableSize() const { return squared_table_size; }
  const std::vector<unsigned>& GetTable() const { return table; }

  // bool IsPrime(uint64_t n) {
  //   assert(n <= squared_table_size);
  //   if (n <= table_size) return (n > 1) && (table[n] == n);
  //   for (size_t i = 0; squared_primes[i] <= n; ++i) {
  //     if ((n % primes[i]) == 0) return false;
  //   }
  //   return true;
  // }
};
}  // namespace table
}  // namespace factorization

using PrimesTable = factorization::table::Primes;
