#include "common/factorization/primes.h"
#include "common/stl/base.h"

int main_007() {
  vector<uint64_t> primes = GeneratePrimes(1000000);
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n;
    cin >> n;
    cout << primes[n - 1] << endl;
  }
  return 0;
}
