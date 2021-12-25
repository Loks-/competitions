#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2110() {
  string ss = "([{<)]}>";
  unordered_map<char, int> um;
  for (int i = 0; i < 8; ++i) um[ss[i]] = i;
  vector<int64_t> vc{3, 57, 1197, 25137};

  vector<int64_t> v;
  auto vs = nvector::ReadLines();
  unsigned r = 0;
  for (auto& s : vs) {
    string snew;
    bool skip = false;
    for (char c : s) {
      int ci = um[c];
      if (ci < 4) {
        snew += c;
      } else if (snew.empty() || (snew.back() != ss[ci - 4])) {
        skip = true;
        r += vc[ci - 4];
        break;
      } else {
        snew.pop_back();
      }
    }
    if (!skip) {
      reverse(snew.begin(), snew.end());
      int64_t x = 0;
      for (char c : snew) x = 5 * x + um[c] + 1;
      v.push_back(x);
    }
  }
  sort(v.begin(), v.end());
  cout << r << endl << v[v.size() / 2] << endl;
  return 0;
}
