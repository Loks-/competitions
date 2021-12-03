#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2101a() {
  unsigned n;
  cin >> n;
  auto v = nvector::Read<unsigned>(n);
  unsigned s = 0;
  for (unsigned i = 1; i < n; ++i) s += (v[i] > v[i - 1]) ? 1 : 0;
  cout << s << endl;
  return 0;
}
