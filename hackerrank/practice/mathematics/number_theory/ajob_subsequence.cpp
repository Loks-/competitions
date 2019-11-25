// https://www.hackerrank.com/challenges/ajob-subsequence

#include "common/modular/proxy/binomial_coefficient_prime.h"
#include "common/stl/base.h"

int main_ajob_subsequence() {
  unsigned T, P;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N, K;
    cin >> N >> K >> P;
    cout << modular::proxy::BinomialCoefficientPrime(N + 1, K + 1, P) << endl;
  }
  return 0;
}
