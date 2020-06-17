#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/utils/digits_sum.h"
#include "common/stl/base.h"

// Time: O(T * N)
// Memory: O(N)
int main_016() {
  unsigned T, N;
  LongUnsigned one(1u);
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    auto l = (one << N);
    cout << DigitsSum(l) << endl;
  }
  return 0;
}
