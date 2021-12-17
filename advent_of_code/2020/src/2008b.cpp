#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"

int main_2008b() {
  auto vs = nvector::TransformV(nvector::ReadLines(),
                                [](auto& s) { return Split(s, ' '); });
  auto Swap = [](string& s) { s = (s == "nop") ? "jmp" : "nop"; };
  int64_t acc = 0;
  for (unsigned iline = 0; iline < vs.size(); ++iline) {
    if (vs[iline][0] == "acc") continue;
    Swap(vs[iline][0]);
    int line = 0;
    acc = 0;
    vector<unsigned> vv(vs.size(), 0);
    for (; (0 <= line) && (line < int(vs.size())); ++line) {
      if (vv[line]) break;
      vv[line] = 1;
      auto& ss = vs[line];
      if (ss[0] == "acc") acc += stoi(ss[1]);
      if (ss[0] == "jmp") line += stoi(ss[1]) - 1;
    }
    if (line == int(vs.size())) break;
    Swap(vs[iline][0]);
  }
  cout << acc << endl;
  return 0;
}
