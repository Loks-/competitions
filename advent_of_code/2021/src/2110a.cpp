#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2110a() {
  string ss = "([{<)]}>";
  unordered_map<char, int> um;
  for (int i = 0; i < 8; ++i) um[ss[i]] = i;

  vector<int64_t> v{3, 57, 1197, 25137};
  auto vs = nvector::ReadLines();
  int64_t r = 0;
  for (auto& s : vs) {
    string snew;
    for (char c : s) {
      int ci = um[c];
      if (ci < 4) {
        snew += c;
      } else if (snew.empty() || (snew.back() != ss[ci - 4])) {
        r += v[ci - 4];
        break;
      } else {
        snew.pop_back();
      }
    }
  }
  cout << r << endl;
  return 0;
}
