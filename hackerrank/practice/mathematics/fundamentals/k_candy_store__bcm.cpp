// https://www.hackerrank.com/challenges/k-candy-store

#include "common/modular/proxy/binomial_coefficient.h"
#include "common/stl/base.h"

int main_k_candy_store__bcm() {
  PModularBinomialCoefficient bcm(1000000000);
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> K;
    cout << bcm.BinomialCoefficient(N + K - 1, K) << endl;
  }
  return 0;
}
