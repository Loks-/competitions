#include "common/stl/base.h"

#include <functional>
#include <string>

int main_alien_rhyme() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    string s;
    vector<string> vs;
    for (unsigned i = 0; i < n; ++i) {
      cin >> s;
      reverse(s.begin(), s.end());
      s.push_back('_');
      vs.push_back(s);
    }
    sort(vs.begin(), vs.end());

    std::function<unsigned(unsigned, unsigned, unsigned)> solve =
        [&](unsigned b, unsigned e, unsigned k) -> unsigned {
      if (e - b <= 1) return 0;
      unsigned r = 0, ib = b;
      for (unsigned i = b + 1; i < e; ++i) {
        if (vs[i][k] != vs[ib][k]) {
          r += solve(ib, i, k + 1);
          ib = i;
        }
      }
      r += solve(ib, e, k + 1);
      if ((k > 0) && (r + 2 <= e - b)) r += 2;
      return r;
    };

    cout << "Case #" << it << ": " << solve(0, unsigned(vs.size()), 0) << endl;
  }
  return 0;
}
