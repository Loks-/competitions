#include "common/numeric/utils/palindrome.h"
#include "common/stl/base.h"

// M = 10^6, L = vp.size()
// Time: O(T * log(L) + M)
// Memory: O(L)
int main_004() {
  vector<uint64_t> vp;
  for (unsigned i = 100; i < 1000; ++i) {
    for (unsigned j = i; j < 1000; ++j) {
      if (Palindrome(i * j)) vp.push_back(i * j);
    }
  }
  sort(vp.begin(), vp.end());

  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n;
    cin >> n;
    auto p = lower_bound(vp.begin(), vp.end(), n);
    cout << *(p - 1) << endl;
  }
  return 0;
}
