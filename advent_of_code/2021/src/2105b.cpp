#include "common/numeric/utils/sign.h"
#include "common/stl/base.h"

#include <unordered_map>

int main_2105b() {
  unsigned n, m = 1000;
  cin >> n;
  string s;
  char c;
  int x0, x1, y0, y1;
  unordered_map<int, unsigned> mc;
  for (unsigned i = 0; i < n; ++i) {
    cin >> x0 >> c >> y0 >> s >> x1 >> c >> y1;
    int dx = x1 - x0, dy = y1 - y0;
    if ((dx == 0) || (dy == 0) || (abs(dx) == abs(dy))) {
      for (int j = 0; j <= max(abs(dx), abs(dy)); ++j)
        mc[x0 + j * Sign(dx) + m * (y0 + j * Sign(dy))] += 1;
    }
  }
  unsigned r = 0;
  for (auto& p : mc) {
    if (p.second >= 2) ++r;
  }
  cout << r << endl;
  return 0;
}
