// https://www.hackerrank.com/challenges/easy-math

#include "common/stl/base.h"

int main_easy_math() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned x;
    cin >> x;
    vector<unsigned> v(x, x);
    v[0] = 0;
    for (unsigned i = 1, t = 0;; ++i) {
      t = (10 * t + 4) % x;
      if (v[t] < x) {
        cout << 2 * i - v[t] << endl;
        break;
      } else {
        v[t] = i;
      }
    }
  }
  return 0;
}
