#include "common/stl/base.h"

#include <string>

int main_power_arrangers() {
  unsigned T, F, n = 119;
  cin >> T >> F;
  for (unsigned it = 1; it <= T; ++it) {
    string s(5 * n, ' '), r, t;
    unsigned l = 24;
    for (unsigned k = 0; k < 4; ++k) {
      vector<unsigned> v(5, 0);
      for (unsigned i = 0; i < n; ++i) {
        if (s.substr(5 * i, k) == r) {
          cout << 5 * i + k + 1 << endl;
          cin >> t;
          char c = t[0];
          s[5 * i + k] = c;
          ++v[c - 'A'];
        }
      }
      for (unsigned i = 0; i < 5; ++i) {
        if ((v[i] == l - 1) && (r.find('A' + i) == string::npos)) {
          r.push_back('A' + char(i));
          break;
        }
      }
      l /= (4 - k);
    }
    for (char c = 'A'; c <= 'E'; ++c) {
      if (r.find(c) == string::npos) {
        r.push_back(c);
        break;
      }
    }
    cout << r << endl;
    cin >> t;
    if (t != "Y") return -1;
  }
  return 0;
}
