#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_set>

int main_2006a() {
  vector<string> vs = nvector::ReadLines();
  vs.push_back("");
  unordered_set<char> cs;
  unsigned r = 0;
  for (auto& s : vs) {
    if (s.empty()) {
      r += cs.size();
      cs.clear();
    } else {
      for (auto c : s) cs.insert(c);
    }
  }
  cout << r << endl;
  return 0;
}
