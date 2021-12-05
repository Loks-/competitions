#include "common/stl/full.h"
#include "common/string/utils/split.h"
#include "common/vector/read.h"

int main_2105b() {
  unsigned n, m = 1000;
  cin >> n;
  auto vs = nvector::Read<string>(3 * n);
  vector<vector<unsigned>> vv(m, vector<unsigned>(m, 0));
  for (unsigned i = 0; i < n; ++i) {
    auto vs1 = Split(vs[3 * i], ',');
    auto vs2 = Split(vs[3 * i + 2], ',');
    int x0 = stoi(vs1[0]);
    int y0 = stoi(vs1[1]);
    int x1 = stoi(vs2[0]);
    int y1 = stoi(vs2[1]);
    if (x0 == x1) {
      if (y0 > y1) swap(y0, y1);
      for (int y = y0; y <= y1; ++y) vv[x0][y] += 1;
    } else if (y0 == y1) {
      if (x0 > x1) swap(x0, x1);
      for (int x = x0; x <= x1; ++x) vv[x][y0] += 1;
    } else if (y1 - y0 == x1 - x0) {
      if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
      }
      for (int i = 0; i <= y1 - y0; ++i) vv[x0 + i][y0 + i] += 1;
    } else if (y1 - y0 == x0 - x1) {
      if (y0 > y1) {
        swap(y0, y1);
        swap(x0, x1);
      }
      for (int i = 0; i <= y1 - y0; ++i) vv[x0 - i][y0 + i] += 1;
    }
  }
  unsigned s = 0;
  for (auto& v : vv) {
    for (auto c : v)
      if (c >= 2) ++s;
  }
  cout << s << endl;
  return 0;
}
