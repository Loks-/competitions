#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2225() {
  unordered_map<char, int> um{
      {'=', -2}, {'-', -1}, {'0', 0}, {'1', 1}, {'2', 2}};
  auto vs = nvector::ReadLines();
  int64_t r = 0;
  for (auto s : vs) {
    int64_t r2 = 0;
    for (char c : s) r2 = 5 * r2 + um[c];
    r += r2;
  }
  vector<char> vc{'=', '-', '0', '1', '2'};
  string s;
  for (; r;) {
    s = vc[(r + 2) % 5] + s;
    r = (r + 2) / 5;
  }
  cout << s << endl;
  return 0;
}
