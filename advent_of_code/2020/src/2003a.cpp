#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2003a() {
  auto vs = nvector::ReadLines();
  unsigned l = vs[0].size(), r = 0;
  for (unsigned i = 0; i < vs.size(); ++i)
    r += (vs[i][(3 * i) % l] == '#') ? 1 : 0;
  cout << r << endl;
  return 0;
}
