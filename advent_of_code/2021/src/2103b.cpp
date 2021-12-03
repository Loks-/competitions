#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2103b() {
  unsigned n;
  cin >> n;
  auto v = nvector::Read<string>(n);
  unsigned l = v[0].size();
  vector<unsigned> vx;
  for (auto s : v) {
    unsigned x = 0;
    for (unsigned i = 0; i < l; ++i) {
      x = 2 * x + (s[i] - '0');
    }
    vx.push_back(x);
  }
  sort(vx.begin(), vx.end());
  uint64_t a = vx[0], b = vx[0];
  for (unsigned i = l - 1, l = 0, r = vx.size(); l + 1 < r; --i) {
    unsigned m = (1u << i), m2 = m * 2;
    unsigned t = (vx[l] & ~(m2 - 1)) + m;
    auto s = lower_bound(vx.begin(), vx.end(), t) - vx.begin();
    if (r - s >= s - l) {
      l = s;
      a = vx[l];
    } else {
      r = s;
    }
  }
  for (unsigned i = l - 1, l = 0, r = vx.size(); l + 1 < r; --i) {
    unsigned m = (1u << i), m2 = m * 2;
    unsigned t = (vx[l] & ~(m2 - 1)) + m;
    auto s = lower_bound(vx.begin(), vx.end(), t) - vx.begin();
    if (r - s >= s - l) {
      r = s;
    } else {
      l = s;
      b = vx[l];
    }
  }
  cout << a * b << endl;
  return 0;
}
