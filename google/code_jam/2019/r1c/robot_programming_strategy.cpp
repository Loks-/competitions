#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_robot_programming_strategy() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    auto vs = nvector::Read<string>(n);
    string strategy;
    for (unsigned i = 0;; ++i) {
      bool r, s, p;
      r = s = p = false;
      for (const string& is : vs) {
        if (is.empty()) continue;
        char c = is[i % is.size()];
        if (c == 'R') r = true;
        if (c == 'S') s = true;
        if (c == 'P') p = true;
      }
      if (r && s && p) {
        strategy.clear();
        break;
      } else if (!r && !s && !p) {
        break;
      } else if (s && !p) {
        strategy.push_back('R');
      } else if (p && !r) {
        strategy.push_back('S');
      } else if (r && !s) {
        strategy.push_back('P');
      }
      for (string& is : vs) {
        if (is.empty()) continue;
        if (is[i % is.size()] != strategy.back()) is.clear();
      }
    }
    cout << "Case #" << it << ": "
         << (strategy.empty() ? "IMPOSSIBLE" : strategy) << std::endl;
  }
  return 0;
}
