#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2006b() {
  vector<string> vs = nvector::ReadLines();
  vs.push_back("");
  unordered_map<char, unsigned> cs;
  unsigned r = 0, l = 0;
  for (auto& s : vs) {
    if (s.empty()) {
      for (auto p : cs) {
        if (p.second == l) ++r;
      }
      cs.clear();
      l = 0;
    } else {
      ++l;
      for (auto c : s) cs[c] += 1;
    }
  }
  cout << r << endl;
  return 0;
}
