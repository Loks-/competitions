// https://www.hackerrank.com/challenges/sherlock-puzzle

#include "common/stl/base.h"

#include <string>

int main_sherlock_puzzle() {
  uint64_t k;
  string s;
  cin >> k >> s;
  vector<int> vs(1, 0), vsmin(1, 0);
  int smin = 0, smax = 0;
  for (unsigned i = 0; i < s.size(); ++i) {
    vs.push_back(vs.back() + ((s[i] == '1') ? 3 : -2));
    smin = min(smin, vs.back());
    smax = max(smax, vs.back());
    vsmin.push_back(smin);
  }
  if (vs.back() >= 0) {
    cout << k * s.size() << endl;
    return 0;
  }
  int max_excess = smax + (vs.back() - smin);
  int maxk = max_excess / -vs.back();
  maxk = min(maxk, int(k) - 2);
  uint64_t maxl = 0;
  for (int fk = max(0, maxk - 1); fk <= maxk; ++fk) {
    for (unsigned i = 0; i < vs.size(); ++i) {
      auto it =
          lower_bound(vsmin.begin(), vsmin.end(), (fk + 1) * vs.back() + vs[i],
                      [](int a, int b) { return a > b; });
      if (it != vsmin.end())
        maxl = max(maxl, uint64_t(fk) * s.size() + i + (vsmin.end() - it - 1));
    }
  }
  if (maxl < s.size()) {
    for (unsigned i = 0; i < vs.size(); ++i) {
      auto it = lower_bound(vsmin.begin(), vsmin.end(), vs[i],
                            [](int a, int b) { return a > b; });
      if (it < vsmin.begin() + i)
        maxl = max(maxl, uint64_t(vsmin.begin() + i - it));
    }
  }
  cout << maxl << endl;
  return 0;
}
