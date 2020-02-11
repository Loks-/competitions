#include "common/numeric/collatz.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

// Time: O(N)
// Memory: O(N)
int main_014() {
  unsigned T;
  cin >> T;
  auto vt = ReadVector<unsigned>(T);
  unsigned N = *max_element(vt.begin(), vt.end());
  unsigned l = 0, r = 0;
  vector<unsigned> vr(1, 0);
  numeric::Collatz c(2 * N);
  for (unsigned i = 1; i <= N; ++i) {
    unsigned il = c(i);
    if (l <= il) {
      l = il;
      r = i;
    }
    vr.push_back(r);
  }
  // cout << c.vs.size() << endl;
  for (auto n : vt) cout << vr[n] << endl;
  return 0;
}
