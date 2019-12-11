// https://www.hackerrank.com/challenges/leonardo-and-prime

#include "common/factorization/primes_generator.h"
#include "common/stl/base.h"

int main_leonardos_prime_factors() {
  vector<uint64_t> primes = GeneratePrimes(100);
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n, k = 0;
    cin >> n;
    for (; primes[k] <= n; ++k) n /= primes[k];
    cout << k << endl;
  }
  return 0;
}
