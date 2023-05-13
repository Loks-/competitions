#include "common/stl/base.h"

#include <string>

int main_untie() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    string s;
    cin >> s;
    unsigned N = s.size();
    bool f = false;
    for (unsigned i = 1; i < N; ++i) {
      if (s[i] != s[i - 1]) {
        f = true;
        reverse(s.begin(), s.begin() + i);
        reverse(s.begin() + i, s.end());
        break;
      }
    }
    unsigned cnt = (f ? 0u : (N + 1) / 2);
    if (f) {
      s.insert(s.begin(), ' ');
      s.push_back(' ');
      for (unsigned i = 0, j = 1; j < s.size(); ++j) {
        if (s[j] == s[i]) continue;
        cnt += (j - i) / 2;
        i = j;
      }
    }
    cout << "Case #" << it << ": " << cnt << endl;
  }
  return 0;
}
