#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2101a() {
  auto v = nvector::ReadAll<unsigned>();
  unsigned s = 0;
  for (unsigned i = 1; i < v.size(); ++i) s += (v[i] > v[i - 1]) ? 1 : 0;
  cout << s << endl;
  return 0;
}
