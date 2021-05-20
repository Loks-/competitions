// https://www.codingame.com/training/easy/horse-racing-duals

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>

int main_horse_racing_duals() {
  unsigned N;
  cin >> N;
  auto v = nvector::Read<unsigned>(N);
  sort(v.begin(), v.end());
  adjacent_difference(v.begin(), v.end(), v.begin());
  cout << *min_element(v.begin() + 1, v.end()) << endl;
  return 0;
}
