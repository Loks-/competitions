#include "common/factorization/primes_generator.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(T * log(N) + N * log(log(N)))
// Memory: O(N)
int main_010() {
  vector<uint64_t> primes = GeneratePrimes(PowU(10u, 6));
  vector<uint64_t> sprimes{0};
  for (uint64_t p : primes) sprimes.push_back(sprimes.back() + p);
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n;
    cin >> n;
    auto itn = upper_bound(primes.begin(), primes.end(), n);
    cout << sprimes[itn - primes.begin()] << endl;
  }
  return 0;
}
