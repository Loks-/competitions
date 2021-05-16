// https://www.codingame.com/training/easy/temperatures

#include "common/stl/base.h"
#include "common/vector/read.h"

int main_temperatures() {
  unsigned N;
  cin >> N;
  auto v = nvector::Read<int>(N);
  cout << (N ? *min_element(v.begin(), v.end(),
                            [](int x, int y) {
                              return (abs(x) < abs(y)) ||
                                     ((abs(x) == abs(y)) && (y < x));
                            })
             : 0)
       << endl;
  return 0;
}
