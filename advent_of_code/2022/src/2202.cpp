#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2202() {
  auto vs = nvector::ReadLines();
  int64_t r1 = 0, r2 = 0;
  for (auto s : vs) {
    int v1 = s[0] - 'A', v2 = s[2] - 'X', v3 = (v2 - v1 + 4) % 3,
        v4 = (v1 + v2 + 2) % 3;
    r1 += v2 + 1 + 3 * v3;
    r2 += v4 + 1 + 3 * v2;
  }
  cout << r1 << endl << r2 << endl;
  return 0;
}
