#include "common/stl/base.h"
#include "common/vector/read_lines.h"
#include "common/vector/split_empty_string.h"

#include <functional>

int main_2213() {
  auto vs = nvector::ReadLines();
  auto vvs = nvector::SplitEmptyString(vs);

  auto SplitI = [](const std::string& s) {
    vector<string> vs;
    if (s.size() == 2) return vs;
    int d = 0;
    unsigned l = 1;
    for (unsigned i = 1; i < s.size(); ++i) {
      if (s[i] == '[') {
        ++d;
      } else if (s[i] == ']') {
        --d;
      } else if ((s[i] == ',') && (d == 0)) {
        vs.push_back(s.substr(l, i - l));
        l = i + 1;
      }
    }
    vs.push_back(s.substr(l, s.size() - l - 1));
    return vs;
  };

  function<int(const string& s1, const string& s2)> Cmp =
      [&](const string& s1, const string& s2) -> int {
    if ((s1[0] != '[') && (s2[0] != '[')) return stoi(s1) - stoi(s2);
    if (s1[0] != '[') return Cmp("[" + s1 + "]", s2);
    if (s2[0] != '[') return Cmp(s1, "[" + s2 + "]");
    auto v1 = SplitI(s1), v2 = SplitI(s2);
    unsigned k = min(v1.size(), v2.size());
    for (unsigned i = 0; i < k; ++i) {
      auto x = Cmp(v1[i], v2[i]);
      if (x) return x;
    }
    return int(v1.size()) - int(v2.size());
  };

  int64_t r1 = 0;
  for (unsigned i = 0; i < vvs.size(); ++i) {
    if (Cmp(vvs[i][0], vvs[i][1]) < 0) r1 += (i + 1);
  }
  cout << r1 << endl;

  vector<string> vs2;
  vs2.push_back("[[2]]");
  vs2.push_back("[[6]]");
  for (auto& s : vs) {
    if (!s.empty()) vs2.push_back("[" + s + "]");
  }
  sort(vs2.begin(), vs2.end(),
       [&](const string& s1, const string& s2) { return Cmp(s1, s2) < 0; });
  unsigned r2 = 1;
  for (unsigned i = 0; i < vs2.size(); ++i) {
    if ((vs2[i] == "[[2]]") || (vs2[i] == "[[6]]")) r2 *= (i + 1);
  }
  cout << r2 << endl;
  return 0;
}
