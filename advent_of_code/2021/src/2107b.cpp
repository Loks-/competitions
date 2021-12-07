#include "common/stl/base.h"
#include "common/vector/read_line.h"

int main_2107b() {
  auto v = nvector::ReadLine<int>(',');
  sort(v.begin(), v.end());
  int64_t r = 1ll << 60;
  for (int m = v[0]; m <= v.back(); ++m) {
    int64_t r0 = 0;
    for (auto x : v) {
      int d = abs(x - m);
      r0 += d * (d + 1) / 2;
    }
    r = min(r, r0);
  }
  cout << r << endl;
  return 0;
}
