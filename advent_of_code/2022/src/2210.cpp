#include "common/numeric/utils/abs.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2210() {
  auto vs = nvector::ReadLines();
  int x = 1, r = 0;
  vector<int> vx;
  for (auto& s : vs) {
    vx.push_back(x);
    auto ss = Split(s, " ");
    if (ss[0] == "addx") {
      vx.push_back(x);
      x += stoi(ss[1]);
    }
  }
  for (unsigned j = 20; j < vx.size(); j += 40) r += j * vx[j - 1];
  cout << r << endl;
  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 40; ++j)
      cout << (Abs(vx[i * 40 + j] - j) <= 1 ? "#" : ".");
    cout << endl;
  }
  return 0;
}
