#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2002a() {
  auto vs = nvector::ReadLines();
  unsigned r = 0;
  for (auto& s : vs) {
    auto ss = Split(s, ' ');
    auto ss0 = Split(ss[0], '-');
    int c = count(ss[2].begin(), ss[2].end(), ss[1][0]);
    if ((c >= stoi(ss0[0])) && (c <= stoi(ss0[1]))) ++r;
  }
  cout << r << endl;
  return 0;
}
