#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

int main_pixelated_circle() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    uint64_t R, total = 0;
    cin >> R;
    for (uint64_t x = 1; x <= R; ++x) total += USqrt(R * R - x * x + R) + 1;
    for (uint64_t r = 1; r <= R; ++r) {
      auto x = USqrt(r * r / 2);
      for (; 2 * x * x + 3 * x + 2 <= r * r;) ++x;
      total -= 2 * x;
      auto y = uint64_t(sqrt(r * r - x * x) + .5);
      if (y > x) total -= 1;
    }
    cout << "Case #" << it << ": " << total * 4 << endl;
  }
  return 0;
}
