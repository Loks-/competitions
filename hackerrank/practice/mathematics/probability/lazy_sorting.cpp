// https://www.hackerrank.com/challenges/lazy-sorting

#include "common/numeric/factorial.h"
#include "common/stl/base.h"

int main_lazy_sorting() {
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
  uint64_t s = GetFactorial(N);
  for (unsigned i : v) s /= GetFactorial(i);
  cout << s << ".000000" << endl;
  return 0;
}
