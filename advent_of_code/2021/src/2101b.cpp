#include "common/stl/full.h"
#include "common/vector/read.h"

int main_2101b() {
  unsigned n;
  cin >> n;
  auto v = nvector::Read<unsigned>(n);
  unsigned s = 0;
  for (unsigned i = 3; i < n; ++i) s += (v[i] > v[i - 3]) ? 1 : 0;
  cout << s << endl;
  return 0;
}
