#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2001b() {
  auto v = nvector::ReadAll<unsigned>();
  for (auto x : v) {
    for (auto y : v) {
      for (auto z : v) {
        if (x + y + z == 2020) {
          cout << x * y * z << endl;
        }
      }
    }
  }
  return 0;
}
