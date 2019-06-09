// https://www.hackerrank.com/challenges/baby-step-giant-step

#include "common/stl/base.h"

int main_baby_step_giant_step() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned a, b, d;
    cin >> a >> b >> d;
    if (a > b) swap(a, b);
    if (d == 0)
      cout << 0 << endl;
    else if ((d == a) || (d == b))
      cout << 1 << endl;
    else if (d < b)
      cout << 2 << endl;
    else
      cout << 1 + (d - 1) / b << endl;
  }
  return 0;
}
