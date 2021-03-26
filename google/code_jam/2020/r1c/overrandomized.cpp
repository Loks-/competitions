#include "common/stl/base.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

int main_overrandomized() {
  unsigned T, U, M = 10000;
  int64_t Q;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> U;
    string s;

    unordered_set<char> cs;
    unordered_map<char, unsigned> cmfd;
    for (unsigned i = 0; i < M; ++i) {
      cin >> Q >> s;
      if (s.size() == U) cmfd[s[0]] += 1;
      for (char c : s) cs.insert(c);
    }
    for (char c : cs) cmfd[c] += 1;
    vector<pair<unsigned, char>> v;
    for (auto p : cmfd) {
      v.push_back({p.second, p.first});
    }
    sort(v.begin(), v.end());
    reverse(v.begin() + 1, v.end());
    cout << "Case #" << it << ": ";
    for (auto p : v) cout << p.second;
    cout << endl;
  }
  return 0;
}
