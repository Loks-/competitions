#include "common/stl/base.h"
#include "common/vector/rotate.h"

int main_join_the_ranks() {
  unsigned T, R, S;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> S;
    unsigned n = (R * (S - 1) + 1) / 2, j, k, l;
    cout << "Case #" << it << ": " << n << endl;
    vector<unsigned> v(R * S + 1, 0);
    for (unsigned i = 0; i < R * S; ++i) v[i] = i % R;
    for (unsigned i = 0; i < n; ++i) {
      for (j = 0; v[j] == v[0];) ++j;
      for (k = j; v[k] == v[j];) ++k;
      for (l = k; v[l] != v[j];) ++l;
      if (l == R * S) {
        k = j;
        l -= (S - k);
      }
      cout << k << " " << l - k << endl;
      SwapIntervals(v.begin(), v.begin() + k, v.begin() + l);
    }
  }
  return 0;
}
