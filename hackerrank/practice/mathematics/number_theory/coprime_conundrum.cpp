// https://www.hackerrank.com/challenges/arthur-and-coprimes

#include "common/factorization/factorization.h"
#include "common/factorization/utils/coprime_count.h"
#include "common/stl/base.h"

int main_coprime_conundrum() {
  unsigned n;
  cin >> n;
  uint64_t total = 0;
  for (unsigned p = 2;; ++p) {
    unsigned maxq = n / p;
    if (maxq <= p) break;
    total += CoprimeCount(Factorize(p), maxq - p);
  }
  cout << total << endl;
  return 0;
}
