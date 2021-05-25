// https://www.codingame.com/training/easy/the-descent

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>

int main_the_descent() {
  for (;;) {
    auto v = nvector::Read<unsigned>(8);
    cout << max_element(v.begin(), v.end()) - v.begin() << endl;
  }
  return 0;
}
