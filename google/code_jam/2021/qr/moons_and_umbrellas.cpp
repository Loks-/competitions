#include "common/stl/base.h"

#include <string>

int main_moons_and_umbrellas() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int cj, jc, tc = 0, tj = 0, l = 2000000;
    string s;
    cin >> cj >> jc;
    cin >> s;
    if (s[0] == 'C')
      tj = l;
    else if (s[0] == 'J')
      tc = l;
    for (unsigned i = 1; i < s.size(); ++i) {
      if (s[i] == 'C') {
        tc = min(tc, tj + jc);
        tj = l;
      } else if (s[i] == 'J') {
        tj = min(tj, tc + cj);
        tc = l;
      } else {  // s[i] == '?'
        int tc_ = tc;
        tc = min(tc, tj + jc);
        tj = min(tj, tc_ + cj);
      }
    }
    cout << "Case #" << it + 1 << ": " << min(tc, tj) << endl;
  }
  return 0;
}
