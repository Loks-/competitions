#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2121a() {
  auto vss = nvector::ReadLines();
  vector<unsigned> vp{unsigned(vss[0].back() - '1'),
                      unsigned(vss[1].back() - '1')},
      vs(2, 0);
  for (unsigned i = 0, dice = 0;; i ^= 1) {
    for (unsigned j = 0; j < 3; ++j) vp[i] += ++dice;
    vs[i] += (vp[i] % 10) + 1;
    if (vs[i] >= 1000u) {
      cout << vs[i ^ 1] * dice << endl;
      break;
    }
  }
  return 0;
}
