#include "common/stl/base.h"

#include <string>

int main_double_or_one_thing() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cout << "Case #" << it << ": ";
    string s;
    cin >> s;
    for (unsigned i = 0; i < s.size(); ++i) {
      cout << s[i];
      bool dr = false;
      for (unsigned j = i + 1; j < s.size(); ++j) {
        dr = s[j] > s[i];
        if (s[j] != s[i]) break;
      }
      if (dr) cout << s[i];
    }
    cout << endl;
  }
  return 0;
}
