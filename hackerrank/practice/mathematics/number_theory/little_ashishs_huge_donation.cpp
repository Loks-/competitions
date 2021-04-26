// https://www.hackerrank.com/challenges/little-chammys-huge-donation

#include "common/algorithm/binary_search.h"
#include "common/stl/base.h"

int main_little_ashishs_huge_donation() {
  auto f = [](uint64_t x) { return (x * (x + 1) * (2 * x + 1)) / 6; };
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N;
    cin >> N;
    cout << UpperBoundF(0, 200000000, N, f) - 1 << endl;
  }
  return 0;
}
