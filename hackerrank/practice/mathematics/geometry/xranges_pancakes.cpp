// https://www.hackerrank.com/challenges/xrange-and-pizza

#include "common/stl/base.h"

int main_xranges_pancakes() {
  unsigned n, m;
  cin >> n >> m;
  unsigned p = 0, f = 0, t, k;
  for (unsigned it = 0; it < m; ++it) {
    cin >> t >> k;
    if (t == 1) {
      p = (p + k) % n;
    } else if (t == 2) {
      p = (n + k - p) % n;
      f ^= 1;
    }
  }
  if (f)
    cout << 2 << " " << p << endl;
  else
    cout << 1 << " " << (n - p) % n << endl;
  return 0;
}
