#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_double_or_noting() {
  unsigned T, mbest = 1000;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    auto vs = nvector::Read<string>(2);
    vector<vector<unsigned>> vv;
    for (auto& s : vs) {
      vector<unsigned> v(1, 1);
      for (unsigned i = 1; i < s.size(); ++i) {
        if (s[i] == s[i - 1])
          v.back() += 1;
        else
          v.push_back(1);
      }
      if (s == "0") v.clear();
      vv.push_back(v);
    }
    auto b = vv[0], e = vv[1];
    unsigned bs = b.size(), es = e.size(), best = mbest;
    // N*
    if (es <= bs) {
      bool einb = true;
      for (unsigned i = 0; i < es; ++i) {
        if (b[i + bs - es] != e[i]) einb = false;
      }
      if (einb) best = min(best, bs - es);
    }
    if (vs[1] == "1") best = min(best, bs + 1);
    // N*D+[ND+]*N?
    if (es & 1) {
      es += 1;
      e.push_back(0);
    }
    for (unsigned i = 0; i < bs; ++i) {
      if ((bs - i + 1) / 2 != (es + 1) / 2) continue;
      vector<unsigned> bi(b.begin() + i, b.end());
      if (bi.size() & 1) bi.push_back(0);
      unsigned bis = bi.size();
      assert(bis == es);
      for (unsigned j = 0; j <= bis; ++j) {
        bool ok = true;
        for (unsigned k = j; k < bis - 1; ++k) {
          if (bi[k] != e[k - j]) ok = false;
        }
        if ((j < bis) && (bi[bis - 1] > e[bis - 1 - j])) ok = false;
        if (ok) {
          unsigned r = (j < bis) ? e[bis - 1 - j] - bi[bis - 1] : 0;
          for (unsigned k = bis - j; k < es; ++k) r += 1 + e[k];
          best = min(best, i + r);
        }
      }
    }
    // B -> "" -> E
    if (es == 2) best = min(best, bs + 1 + ((e[0] == 1) ? 0 : e[0] + 1) + e[1]);
    cout << "Case #" << it << ": "
         << ((best < mbest) ? to_string(best) : "IMPOSSIBLE") << endl;
  }
  return 0;
}
