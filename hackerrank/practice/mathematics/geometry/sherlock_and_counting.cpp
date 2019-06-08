// https://www.hackerrank.com/challenges/sherlock-and-counting

#include "common/stl/base.h"

#include <cmath>

int main_sherlock_and_counting() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int64_t n, k, i;
    cin >> n >> k;
    i = n / 2;
    if (i * (n - i) <= n * k) {
      cout << n - 1 << endl;
    } else {
      double d = n * (n - 4 * k);
      double x = 0.5 * (n - sqrt(d));
      cout << 2 * int(x) << endl;
    }
  }
  return 0;
}
