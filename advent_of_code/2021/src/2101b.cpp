#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2101b() {
  auto v = nvector::ReadAll<unsigned>();
  unsigned s = 0;
  for (unsigned i = 3; i < v.size(); ++i) s += (v[i] > v[i - 3]) ? 1 : 0;
  cout << s << endl;
  return 0;
}
