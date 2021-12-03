#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2003a() {
  unsigned n;
  cin >> n;
  auto v = nvector::Read<string>(n);
  unsigned l = v[0].size(), s = 0;
  for (unsigned i = 0; i < n; ++i) {
    s += (v[i][(3 * i) % l] == '#') ? 1 : 0;
  }
  cout << s << endl;
  return 0;
}
