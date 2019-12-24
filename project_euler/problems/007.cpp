#include "common/factorization/primes_generator.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(T + N * log N * log log N)
// Memory: O(N)
int main_007() {
  vector<uint64_t> primes = GeneratePrimes(PowU(10u, 6));
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n;
    cin >> n;
    cout << primes[n - 1] << endl;
  }
  return 0;
}
