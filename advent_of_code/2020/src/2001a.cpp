#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2001a() {
  auto v = nvector::ReadAll<unsigned>();
  for (auto x : v) {
    for (auto y : v) {
      if (x + y == 2020) {
        cout << x * y << endl;
      }
    }
  }
  return 0;
}
