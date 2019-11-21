// https://www.hackerrank.com/challenges/game-of-throne-ii

#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

#include <string>

using TFactorial = ModularFactorial<TModularD, false>;

int main_game_of_thrones_ii() {
  TFactorial f;
  string s;
  cin >> s;
  sort(s.begin(), s.end());
  unsigned n = unsigned(s.size());
  s.push_back(' ');
  unsigned ls = 0, so = 0;
  TModularD r = 1;
  for (unsigned i = 1; i <= n; ++i) {
    if (s[i] == s[i - 1]) continue;
    unsigned l = i - ls;
    ls = i;
    so += (l & 1);
    r *= f(l / 2);
  }
  cout << ((so > 1) ? TModularD(0) : f(n / 2) / r) << endl;
  return 0;
}
