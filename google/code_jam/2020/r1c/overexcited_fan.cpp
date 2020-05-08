#include "common/stl/base.h"

#include <string>

int main_overexcited_fan() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int x, y, l = 0;
    string s;
    cin >> x >> y >> s;
    for (int i = 1; i <= int(s.size()); ++i) {
      switch (s[i - 1]) {
        case 'N':
          ++y;
          break;
        case 'S':
          --y;
          break;
        case 'E':
          ++x;
          break;
        case 'W':
          --x;
          break;
      }
      if (abs(x) + abs(y) <= i) {
        l = i;
        break;
      }
    }
    cout << "Case #" << it << ": " << (l ? to_string(l) : "IMPOSSIBLE") << endl;
  }
  return 0;
}
