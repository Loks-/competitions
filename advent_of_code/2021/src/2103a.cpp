#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2103a() {
  unsigned n;
  cin >> n;
  auto v = nvector::Read<string>(n);
  unsigned l = v[0].size();
  vector<unsigned> vc(l, 0);
  for (auto s : v) {
    for (unsigned i = 0; i < l; ++i) {
      if (s[i] == '1') vc[i] += 1;
    }
  }
  uint64_t x = 0;
  for (unsigned i = 0; i < l; ++i) {
    x *= 2;
    if (vc[i] >= n / 2) x += 1;
  }
  uint64_t y = ((1ull << l) - 1) ^ x;
  cout << x * y << endl;
  return 0;
}
