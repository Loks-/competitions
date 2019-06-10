// https://www.hackerrank.com/challenges/circle-city

#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

int main_circle_city() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned d, k;
    cin >> d >> k;
    unsigned i = 0, j = USqrt(d), s = 0;
    for (; i <= j;) {
      if (i * i + j * j < d) {
        ++i;
      } else if (i * i + j * j > d) {
        --j;
      } else {
        s += ((i == 0) || (i == j)) ? 4 : 8;
        ++i;
        --j;
      }
    }
    cout << ((s <= k) ? "possible" : "impossible") << endl;
  }
  return 0;
}
