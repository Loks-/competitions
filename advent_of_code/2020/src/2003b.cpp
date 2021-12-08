#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2003b() {
  auto vs = nvector::ReadLines();
  unsigned l = vs[0].size();
  auto Solve = [&](unsigned dx, unsigned dy) {
    unsigned s = 0;
    for (unsigned i = 0; i < vs.size(); i += dy)
      s += (vs[i][(dx * (i / dy)) % l] == '#') ? 1 : 0;
    return s;
  };

  uint64_t r = 1;
  r *= Solve(1, 1);
  r *= Solve(3, 1);
  r *= Solve(5, 1);
  r *= Solve(7, 1);
  r *= Solve(1, 2);
  cout << r << endl;
  return 0;
}
