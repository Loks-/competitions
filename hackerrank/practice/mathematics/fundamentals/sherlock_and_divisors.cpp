// https://www.hackerrank.com/challenges/sherlock-and-divisors

#include "common/factorization/factorization.h"
#include "common/factorization/utils/divisors_count.h"
#include "common/stl/base.h"

int main_sherlock_and_divisors() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    cout << ((N & 1) ? 0 : DivisorsCount(Factorize(N / 2))) << endl;
  }
  return 0;
}
