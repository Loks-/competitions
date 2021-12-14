#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <numeric>

int main_2114b() {
  auto vs = nvector::ReadLines();
  auto s = vs[0] + 'Z';
  unsigned l = 'Z' - 'A' + 1;
  vector<vector<uint64_t>> v0(l, vector<uint64_t>(l, 0)),
      vm(l, vector<uint64_t>(l, l));
  for (unsigned i = 1; i < s.size(); ++i) v0[s[i - 1] - 'A'][s[i] - 'A'] += 1;
  for (unsigned i = 2; i < vs.size(); ++i) {
    auto ss = Split(vs[i], ' ');
    vm[ss[0][0] - 'A'][ss[0][1] - 'A'] = (ss[2][0] - 'A');
  }
  auto v1 = v0;
  for (unsigned im = 0; im < 40; ++im) {
    for (unsigned i = 0; i < l; ++i) {
      for (unsigned j = 0; j < l; ++j) {
        unsigned k = vm[i][j];
        if (k < l) {
          v1[i][k] += v0[i][j];
          v1[k][j] += v0[i][j];
          v1[i][j] -= v0[i][j];
        }
      }
    }
    v0 = v1;
  }
  vector<uint64_t> cnt;
  for (unsigned i = 0; i < l - 1; ++i) {
    uint64_t r = accumulate(v0[i].begin(), v0[i].end(), 0ull);
    if (r) cnt.push_back(r);
  }
  sort(cnt.begin(), cnt.end());
  cout << cnt.back() - cnt[0] << endl;
  return 0;
}
