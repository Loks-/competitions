#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

int main_letter_blocks() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    auto vs = nvector::Read<string>(N);
    unordered_map<char, string> m1;
    for (unsigned i = 0; i < vs.size(); ++i) {
      if (vs[i] == string(vs[i].size(), vs[i][0])) {
        m1[vs[i][0]] += vs[i];
        vs[i--] = vs.back();
        vs.pop_back();
      }
    }
    for (unsigned i = 0; i < vs.size(); ++i) {
      string s = vs[i];
      char f = s[0], l = s.back();
      s = m1[f] + s;
      m1[f].clear();
      s += m1[l];
      m1[l].clear();
      for (unsigned j = i + 1; j < vs.size(); ++j) {
        if (vs[j][0] == l) {
          s += vs[j];
          l = s.back();
          vs[j] = vs.back();
          j = i;
          vs.pop_back();
          s += m1[l];
          m1[l].clear();
        } else if (vs[j].back() == f) {
          s = vs[j] + s;
          f = s[0];
          vs[j] = vs.back();
          vs.pop_back();
          j = i;
          s = m1[f] + s;
          m1[f].clear();
        }
      }
      vs[i] = s;
    }
    string sf = "";
    for (auto& s : vs) sf += s;
    for (auto& p : m1) sf += p.second;
    unordered_set<char> s1;
    s1.insert(sf[0]);
    for (unsigned i = 1; i < sf.size(); ++i) {
      if (sf[i] == sf[i - 1]) continue;
      if (s1.find(sf[i]) != s1.end()) {
        sf = "IMPOSSIBLE";
        break;
      }
      s1.insert(sf[i]);
    }
    cout << "Case #" << it << ": " << sf << endl;
  }
  return 0;
}
