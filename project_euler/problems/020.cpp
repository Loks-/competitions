#include "common/numeric/long/utils/digits_sum.h"
#include "common/numeric/long/utils/factorial.h"
#include "common/stl/base.h"

// Time: O(T * N^2)
// Memory: O(N)
int main_020() {
  unsigned T, N;
  LongUnsigned one(1u);
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    cout << DigitsSum(GetFactorialL(N)) << endl;
  }
  return 0;
}
