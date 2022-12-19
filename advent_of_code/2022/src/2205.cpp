#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/extract_int.h"
#include "common/vector/read_lines.h"

int main_2205() {
  auto vs = nvector::ReadLines();
  unsigned l = 0;
  for (; !vs[l].empty();) ++l;
  auto vi = Split(vs[l - 1], " ");
  for (bool ab : {false, true}) {
    vector<vector<char>> vv(vi.size() + 1);
    for (unsigned i = l - 1; i--;) {
      auto s = vs[i];
      s.resize(4 * vv.size(), ' ');
      for (unsigned j = 1; j < vv.size(); ++j) {
        if (s[4 * j - 3] != ' ') vv[j].push_back(s[4 * j - 3]);
      }
    }
    for (unsigned i = l + 1; i < vs.size(); ++i) {
      auto vi = nvector::ExtractInt<unsigned>(vs[i], " ");
      unsigned n = vi[0], f = vi[1], t = vi[2];
      if ((vv[f].size() < n) || (f == t)) cout << "X" << endl;
      if (ab)
        vv[t].insert(vv[t].end(), vv[f].end() - n, vv[f].end());
      else
        vv[t].insert(vv[t].end(), vv[f].rbegin(), vv[f].rbegin() + n);
      vv[f].resize(vv[f].size() - n);
    }
    for (unsigned i = 1; i < vv.size(); ++i) cout << vv[i].back();
    cout << endl;
  }
  return 0;
}
