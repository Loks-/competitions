#include "common/stl/base.h"
#include "common/vector/read_from_string.h"
#include "common/vector/read_lines.h"

int main_2204() {
  auto vs = nvector::ReadLines();
  unsigned r1 = 0, r2 = 0;
  for (auto& s : vs) {
    auto v = nvector::ReadFromString<int>(s, "-,");
    if (((v[0] - v[2]) * (v[1] - v[3])) <= 0) r1 += 1;
    if (min(v[1], v[3]) >= max(v[0], v[2])) r2 += 1;
  }
  cout << r1 << endl << r2 << endl;
  return 0;
}
