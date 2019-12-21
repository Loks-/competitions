#include "common/factorization/primes_generator.h"
#include "common/stl/base.h"

int main_010() {
  vector<uint64_t> primes = GeneratePrimes(1000000);
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
