#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2004a() {
  vector<string> vsi = nvector::ReadLines(), vs(1, "");
  for (auto& s : vsi) {
    if (s.empty())
      vs.push_back("");
    else
      vs.back() = vs.back() + " " + s;
  }
  unsigned r = 0;
  for (auto& s1 : vs) {
    auto ss = Split(s1, ' ');
    unsigned f = 0;
    for (auto& s2 : ss) {
      if (s2.empty()) continue;
      if (s2.substr(0, 3) != "cid") ++f;
    }
    if (f >= 7) ++r;
  }
  cout << r << endl;
  return 0;
}
