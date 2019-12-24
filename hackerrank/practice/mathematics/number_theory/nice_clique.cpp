// https://www.hackerrank.com/challenges/niceclique

#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/primes_powers.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_nice_clique() {
  PrimesTable primes_table(100000);
  unsigned n;
  cin >> n;
  vector<uint64_t> v = ReadVector<uint64_t>(n);
  vector<unsigned> m1(2), m2(2);
  for (uint64_t x : v) {
    auto f = PrimesPowers(primes_table, x);
    m1[f.size() & 1] += 1;
    uint64_t i = (x & 1) ^ 1;
    bool square = true;
    for (; i < f.size(); ++i) {
      if (f[i] & 1) {
        square = false;
        break;
      }
    }
    m2[square ? 1 : 0] += 1;
  }
  cout << max(max(m1[0], m1[1]), max(m2[0], m2[1])) << endl;
  return 0;
}
