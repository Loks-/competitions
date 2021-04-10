#include "common/stl/base.h"

#include <string>

int main_append_sort() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned r = 0;
    std::string s0, s1;
    cin >> N >> s0;
    for (unsigned i = 1; i < N; ++i) {
      cin >> s1;
      if ((s1.size() < s0.size()) || ((s1.size() == s0.size()) && (s1 <= s0))) {
        unsigned l = s1.size();
        auto s0l = s0.substr(0, l);
        if (s0l < s1) {
          r += s0.size() - s1.size();
          s1.resize(s0.size(), '0');
        } else if (s0l > s1) {
          r += s0.size() - s1.size() + 1;
          s1.resize(s0.size() + 1, '0');
        } else {
          auto se = s0.substr(l);
          if (se == std::string(se.size(), '9')) {
            r += s0.size() - s1.size() + 1;
            s1.resize(s0.size() + 1, '0');
          } else {
            r += s0.size() - s1.size();
            s1 = s0;
            unsigned i = s1.size() - 1;
            for (; s1[i] == '9';) --i;
            s1[i++] += 1;
            for (; i < s1.size(); ++i) s1[i] = '0';
          }
        }
      }
      s0 = s1;
    }
    cout << "Case #" << it << ": " << r << endl;
  }
  return 0;
}
