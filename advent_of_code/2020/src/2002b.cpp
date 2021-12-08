#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2002b() {
  auto vs = nvector::ReadLines();
  unsigned r = 0;
  for (auto& s : vs) {
    auto ss = Split(s, ' ');
    auto ss0 = Split(ss[0], '-');
    if ((ss[2][stoi(ss0[0]) - 1] == ss[1][0]) !=
        (ss[2][stoi(ss0[1]) - 1] == ss[1][0]))
      ++r;
  }
  cout << r << endl;
  return 0;
}
