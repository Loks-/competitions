// https://www.hackerrank.com/challenges/harry-potter-and-the-floating-rocks

#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

int main_sumar_and_the_floating_rocks() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    cout << GCDSafe(x1 - x2, y1 - y2) - 1 << endl;
  }
  return 0;
}
