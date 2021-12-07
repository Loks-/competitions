#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/transform.h"

int main_2107b() {
  string s;
  cin >> s;
  auto v = nvector::TransformV(Split(s, ','), [](auto& s) { return stoi(s); });
  sort(v.begin(), v.end());
  unsigned n = v.size();
  int64_t r = 1ll << 60;
  for (int m = v[0]; m <= v.back(); ++m) {
    int64_t r0 = 0;
    for (auto x : v) {
      int d = abs(x - m);
      r0 += d * (d + 1) / 2;
    }
    r = min(r, r0);
  }
  cout << n << " " << r << endl;
  return 0;
}
