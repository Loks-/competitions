// https://www.hackerrank.com/challenges/gcd-product

#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_gcd_product__counting() {
  uint64_t n, m;
  cin >> n >> m;
  if (n < m) swap(n, m);
  vector<uint64_t> vc(m + 1);
  for (uint64_t i = 1; i <= m; ++i) vc[i] = (n / i) * (m / i);
  vector<unsigned> vskip(max(n, uint64_t(4000)) + 1, 1);
  for (uint64_t i = 2; i <= 4000; ++i) vskip[i] = 0;
  for (uint64_t k = 1; 4000 * k <= n; ++k) vskip[m / k] = vskip[n / k] = 0;
  for (uint64_t i = m / 2; i > 1; --i) {
    if (vskip[i])
      vc[i] = vc[i + 1];
    else
      for (uint64_t j = 2 * i; j <= m; j += i) vc[i] -= vc[j];
  }
  TModular r = 1;
  for (uint64_t i = 2; i <= m; ++i) r *= TModular(i).PowU(vc[i]);
  cout << r << endl;
  return 0;
}
