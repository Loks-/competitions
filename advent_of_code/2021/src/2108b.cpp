#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2108b() {
  vector<string> vi{"abcefg", "cf",     "acdeg", "acdfg",   "bcdf",
                    "abdfg",  "abdefg", "acf",   "abcdefg", "abcdfg"};
  unordered_map<string, unsigned> m;
  for (unsigned i = 0; i < vi.size(); ++i) m[vi[i]] = i;

  auto vs = nvector::ReadLines();
  unsigned r = 0;
  for (auto s : vs) {
    auto ss = Split(s, ' ');
    vector<unsigned> vp{0, 1, 2, 3, 4, 5, 6};
    for (;; next_permutation(vp.begin(), vp.end())) {
      auto Apply = [&](string w) {
        for (char& c : w) c = 'a' + vp[c - 'a'];
        sort(w.begin(), w.end());
        return w;
      };

      bool ok = true;
      for (auto w : ss) {
        if (w == "|") continue;
        if (m.find(Apply(w)) == m.end()) {
          ok = false;
          break;
        }
      }
      if (ok) {
        unsigned x = 0;
        for (unsigned i = 11; i < 15; ++i) x = x * 10 + m[Apply(ss[i])];
        r += x;
        break;
      }
    }
  }
  cout << r << endl;
  return 0;
}
