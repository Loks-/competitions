#include "common/stl/base.h"
#include "common/stl/pair_io.h"
#include "common/vector/read_all.h"

int main_2102() {
  int64_t x = 0, y = 0, a = 0;
  for (auto& p : nvector::ReadAll<pair<string, int>>()) {
    if (p.first == "forward") {
      x += p.second;
      y += p.second * a;
    }
    if (p.first == "down") a += p.second;
    if (p.first == "up") a -= p.second;
  }
  cout << x * a << endl;
  cout << x * y << endl;
  return 0;
}
