#include "common/stl/base.h"

#include <string>

int main_moons_and_umbrellas() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int cj, jc, cjc, tc = 0;
    cin >> cj >> jc;
    cjc = cj + jc;

    auto SolveBB = [&](int n) {
      assert(n > 1);
      if ((cj < 0) || (jc < 0)) {
        if (cjc < 0) {
          int c = cjc * ((n - 2) / 2);
          if ((n & 1)) c += min(cj, jc);
          return c;
        } else if (n > 2) {
          return min(cj, jc);
        }
      }
      return 0;
    };

    auto SolveBC = [&](int n) {
      int c = 0;
      if (cjc < 0) {
        c = cjc * ((n - 1) / 2);
        if ((n & 1) == 0) c += min(jc, 0);
      }
      return min(((n > 1) ? jc : 0), c);
    };

    auto SolveBJ = [&](int n) {
      int c = 0;
      if (cjc < 0) {
        c = cjc * ((n - 1) / 2);
        if ((n & 1) == 0) c += min(cj, 0);
      }
      return min(((n > 1) ? cj : 0), c);
    };

    auto SolveCB = [&](int n) { return SolveBJ(n); };
    auto SolveJB = [&](int n) { return SolveBC(n); };
    auto SolveCC = [&](int n) { return (n / 2) * min(cjc, 0); };
    auto SolveCJ = [&](int n) { return cj + SolveCC(n - 1); };
    auto SolveJC = [&](int n) { return jc + SolveCC(n - 1); };
    auto SolveJJ = [&](int n) { return SolveCC(n); };

    string s;
    cin >> s;
    s = "B" + s;
    unsigned p0 = 0, p1 = 1;
    for (; p1 < s.size(); ++p1) {
      if (s[p1] == 'C') {
        if (s[p0] == 'C')
          tc += SolveCC(p1 - p0);
        else if (s[p0] == 'J')
          tc += SolveJC(p1 - p0);
        else
          tc += SolveBC(p1 - p0);
        p0 = p1;
      } else if (s[p1] == 'J') {
        if (s[p0] == 'C')
          tc += SolveCJ(p1 - p0);
        else if (s[p0] == 'J')
          tc += SolveJJ(p1 - p0);
        else
          tc += SolveBJ(p1 - p0);
        p0 = p1;
      }
    }
    if (s[p0] == 'C')
      tc += SolveCB(p1 - p0);
    else if (s[p0] == 'J')
      tc += SolveJB(p1 - p0);
    else
      tc += SolveBB(p1 - p0);

    cout << "Case #" << it + 1 << ": " << tc << endl;
  }
  return 0;
}
