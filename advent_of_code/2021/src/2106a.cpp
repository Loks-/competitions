#include "common/stl/base.h"
#include "common/string/utils/split.h"

#include <numeric>

int main_2106a() {
  unsigned m = 80;
  string s;
  cin >> s;
  auto vs = Split(s, ',');
  vector<uint64_t> vc(9, 0), vc2(9);
  for (auto ss : vs) {
    int x = stoi(ss);
    vc[x] += 1;
  }
  for (unsigned im = 0; im < m; ++im) {
    for (unsigned i = 0; i < 8; ++i) {
      vc2[i] = vc[i + 1];
    }
    vc2[8] = vc[0];
    vc2[6] += vc[0];
    vc.swap(vc2);
  }
  cout << accumulate(vc.begin(), vc.end(), 0ull) << endl;
  return 0;
}
