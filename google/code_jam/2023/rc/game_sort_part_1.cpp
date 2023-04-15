#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_game_sort_part_1() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    bool ok = true;
    cin >> N;
    auto vs = nvector::Read<std::string>(N);
    sort(vs[0].begin(), vs[0].end());
    for (unsigned i = 1; ok && (i < N); ++i) {
      auto &s0 = vs[i - 1], &s1 = vs[i];
      string s;
      sort(s1.begin(), s1.end());
      for (unsigned j = 0; j < s0.size(); ++j) {
        auto it = lower_bound(s1.begin(), s1.end(), s0[j]);
        if (it == s1.end()) {
          reverse(s1.begin(), s1.end());
          s += s1;
          s1.clear();
          next_permutation(s.begin(), s.end());
          break;
        } else if (*it == s0[j]) {
          s += *it;
          s1.erase(it);
        } else {
          s += *it;
          s1.erase(it);
          s += s1;
          s1.clear();
          break;
        }
      }
      s += s1;
      s1 = s;
      ok = (s0 <= s1);
    }
    cout << "Case #" << it << ": " << (ok ? "POSSIBLE" : "IMPOSSIBLE")
         << std::endl;
    if (ok) {
      for (auto s : vs) cout << s << " ";
      cout << std::endl;
    }
  }
  return 0;
}
