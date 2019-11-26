// https://www.hackerrank.com/challenges/lazy-sorting

#include "common/numeric/factorial.h"
#include "common/stl/base.h"

int main_lazy_sorting() {
  numeric::Factorial f;
  unsigned N, x, lx = 0;
  cin >> N;
  vector<unsigned> v(101, 0);
  bool sorted = true;
  for (unsigned it = 0; it < N; ++it) {
    cin >> x;
    if (x < lx) sorted = false;
    lx = x;
    ++v[x];
  }
  if (sorted) {
    cout << "0.000000" << endl;
    return 0;
  }
  uint64_t s = f(N);
  for (unsigned i : v) s /= f(i);
  cout << s << ".000000" << endl;
  return 0;
}
