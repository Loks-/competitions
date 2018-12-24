// https://www.hackerrank.com/challenges/x-and-his-shots

#include "common/stl/base.h"

int main_mr_x_and_his_shots() {
  unsigned n, m, a, b;
  cin >> n >> m;
  vector<unsigned> va, vb;
  for (unsigned i = 0; i < n; ++i) {
    cin >> a >> b;
    va.push_back(a);
    vb.push_back(b);
  }
  sort(va.begin(), va.end());
  sort(vb.begin(), vb.end());
  uint64_t s = 0;
  for (unsigned i = 0; i < m; ++i) {
    cin >> a >> b;
    s += (upper_bound(va.begin(), va.end(), b) - va.begin());
    s -= (lower_bound(vb.begin(), vb.end(), a) - vb.begin());
  }
  cout << s << endl;
  return 0;
}
