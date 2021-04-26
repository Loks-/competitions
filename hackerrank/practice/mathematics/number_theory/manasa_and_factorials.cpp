// https://www.hackerrank.com/challenges/manasa-and-factorials

#include "common/algorithm/binary_search.h"
#include "common/stl/base.h"

int main_manasa_and_factorials() {
  auto f = [](uint64_t x) {
    uint64_t s = 0;
    for (x /= 5; x; x /= 5) s += x;
    return s;
  };

  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t N;
    cin >> N;
    cout << LowerBoundF(0ull, 50000000000000000ull, N, f) << endl;
  }
  return 0;
}
