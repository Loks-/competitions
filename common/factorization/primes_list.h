#pragma once

#include "common/factorization/base.h"

#include <algorithm>
#include <vector>

namespace factorization {
class PrimesList {
 protected:
  uint64_t table_size, squared_table_size;
  std::vector<uint64_t> primes, squared_primes;
  std::vector<unsigned> table;

 public:
  constexpr explicit PrimesList(uint64_t size) {
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

  constexpr const std::vector<uint64_t>& GetPrimes() const { return primes; }

  constexpr const std::vector<uint64_t>& GetSquaredPrimes() const {
    return squared_primes;
  }

  constexpr uint64_t GetTableSize() const { return table_size; }
  constexpr uint64_t GetSquaredTableSize() const { return squared_table_size; }
  constexpr const std::vector<unsigned>& GetTable() const { return table; }

  constexpr bool IsPrime(uint64_t n) {
    assert(n <= squared_table_size);
    if (n <= table_size) return (n > 1) && (table[n] == n);
    for (size_t i = 0; squared_primes[i] <= n; ++i) {
      if ((n % primes[i]) == 0) return false;
    }
    return true;
  }

  constexpr TFactorization FactorizeTable(uint64_t n) const {
    assert(n <= table_size);
    TFactorization output;
    for (; n > 1;) {
      const unsigned p = table[n];
      unsigned cnt = 1;
      for (n /= p; table[n] == p; ++cnt) n /= p;
      output.push_back({uint64_t(p), cnt});
    }
    return output;
  }

  constexpr TFactorization Factorize(uint64_t n,
                                     bool skip_assert = false) const {
    if (n <= table_size) return FactorizeTable(n);
    TFactorization output;
    for (uint64_t p : primes) {
      if (n < p * p) break;
      if ((n % p) == 0) {
        n /= p;
        unsigned cnt = 1;
        for (; (n % p) == 0; ++cnt) n /= p;
        output.push_back({p, cnt});
      }
    }
    if (!skip_assert) assert(squared_table_size > n);
    if (n != 1) output.push_back({n, 1});
    return output;
  }
};
}  // namespace factorization
