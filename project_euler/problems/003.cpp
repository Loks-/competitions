#include "common/factorization/factorization.h"
#include "common/stl/base.h"

// Time: O(T * N^(1/4))
// Memory: O(1)
int main_003() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n;
    cin >> n;
    auto f = Factorize(n);
    cout << f.back().prime << endl;
  }
  return 0;
}
