#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

int main_2108a() {
  unsigned r = 0;
  for (auto s : nvector::ReadLines()) {
    auto ss = Split(s, ' ');
    for (unsigned i = 11; i < ss.size(); ++i) {
      unsigned l = ss[i].size();
      if ((l < 5) || (l == 7)) ++r;
    }
  }
  cout << r << endl;
  return 0;
}
