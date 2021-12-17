#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2008a() {
  auto vs = nvector::ReadLines();
  int64_t acc = 0;
  vector<unsigned> vv(vs.size(), 0);
  for (unsigned line = 0; line < vs.size(); ++line) {
    if (vv[line]) break;
    vv[line] = 1;
    auto ss = Split(vs[line], ' ');
    if (ss[0] == "acc") acc += stoi(ss[1]);
    if (ss[0] == "jmp") line += stoi(ss[1]) - 1;
  }
  cout << acc << endl;
  return 0;
}
