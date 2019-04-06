#include "common/stl/base.h"

int main_foregone_solution() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    string s1;
    cin >> s1;
    string s2 = string(s1.size(), '0');
    for (unsigned i = 0; i < s1.size(); ++i) {
      if (s1[i] == '4') {
        s1[i] = '3';
        s2[i] = '1';
      }
    }
    size_t npos = s2.find('1');
    s2 = s2.substr(npos);
    cout << "Case #" << it + 1 << ": " << s1 << " " << s2 << endl;
  }
  return 0;
}
