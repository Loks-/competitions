#include "common/numeric/long/unsigned_io.h"
#include "common/stl/base.h"

// Time: O(N)
// Memory: O(1)
int main_013() {
  unsigned N;
  cin >> N;
  LongUnsigned s, t;
  for (unsigned i = 0; i < N; ++i) {
    cin >> t;
    s += t;
  }
  cout << ToString(s).substr(0, 10) << endl;
  return 0;
}
