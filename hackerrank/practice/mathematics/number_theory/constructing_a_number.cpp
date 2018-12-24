// https://www.hackerrank.com/challenges/constructing-a-number

#include "common/stl/base.h"

int main_constructing_a_number() {
  unsigned T, n, a, s;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    s = 0;
    cin >> n;
    for (unsigned in = 0; in < n; ++in) {
      cin >> a;
      s += (a % 3);
    }
    cout << ((s % 3) ? "No" : "Yes") << endl;
  }
  return 0;
}
