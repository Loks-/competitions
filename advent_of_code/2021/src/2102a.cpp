#include "common/stl/base.h"
#include "common/stl/pair_io.h"
#include "common/vector/read_all.h"

int main_2102a() {
  int x = 0, y = 0;
  for (auto& p : nvector::ReadAll<pair<string, int>>()) {
    if (p.first == "forward") x += p.second;
    if (p.first == "down") y += p.second;
    if (p.first == "up") y -= p.second;
  }
  cout << x * y << endl;
  return 0;
}
