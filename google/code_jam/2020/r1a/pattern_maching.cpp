#include "common/stl/base.h"
#include "common/string/end_with.h"
#include "common/string/erase_all.h"
#include "common/string/start_with.h"

#include <string>

int main_pattern_maching() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    vector<string> vp, vs;
    string m, s;
    for (unsigned i = 0; i < N; ++i) {
      cin >> s;
      size_t npos1 = s.find('*'), npos2 = s.rfind('*');
      vp.push_back(s.substr(0, npos1));
      vs.push_back(s.substr(npos2 + 1));
      m += EraseAll(s.substr(npos1, npos2), '*');
    }
    std::sort(vp.begin(), vp.end(),
              [](auto& s1, auto& s2) { return s1.size() < s2.size(); });
    std::sort(vs.begin(), vs.end(),
              [](auto& s1, auto& s2) { return s1.size() < s2.size(); });
    bool correct = true;
    for (unsigned i = 1; i < N; ++i) {
      if (!StartWith(vp[i], vp[i - 1]) || !EndWith(vs[i], vs[i - 1]))
        correct = false;
    }
    cout << "Case #" << it << ": "
         << (correct ? vp.back() + m + vs.back() : "*") << endl;
  }
  return 0;
}
