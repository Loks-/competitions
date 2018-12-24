// https://www.hackerrank.com/challenges/gcd-product

#include "common/factorization/primes_list.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_gcd_product() {
  uint64_t n, m;
  cin >> n >> m;
  if (n < m) swap(n, m);
  TModular r = 1;
  PrimesList primes_list(m);
  for (uint64_t p : primes_list.GetPrimes()) {
    for (uint64_t pp = p; pp <= m; pp *= p)
      r *= TModular(p).PowU((m / pp) * (n / pp));
  }
  cout << r << endl;
  return 0;
}
