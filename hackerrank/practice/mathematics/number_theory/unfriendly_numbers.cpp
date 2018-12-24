// https://www.hackerrank.com/challenges/unfriendly-numbers

#include "common/factorization/factorization.h"
#include "common/factorization/utils/divisors.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

#include <unordered_set>

int main_unfriendly_numbers() {
  uint64_t n, f, u;
  cin >> n >> f;
  unordered_set<uint64_t> sun;
  for (uint64_t i = 0; i < n; ++i) {
    cin >> u;
    u = GCD(u, f);
    sun.insert(u);
  }
  vector<uint64_t> un(sun.begin(), sun.end());
  sort(un.begin(), un.end());
  reverse(un.begin(), un.end());

  vector<uint64_t> fd = GetDivisors(FastFactorize(f));
  unsigned ud = 0;
  for (uint64_t d : fd) {
    if (d > un[0]) {
      ++ud;
      continue;
    }
    bool good = true;
    for (uint64_t u : un) {
      if (u < d) break;
      if ((u % d) == 0) {
        good = false;
        break;
      }
    }
    if (good) ++ud;
  }
  cout << ud << endl;
  return 0;
}
