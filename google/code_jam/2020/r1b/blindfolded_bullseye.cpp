#include "common/stl/base.h"

#include <string>

int main_blindfolded_bullseye() {
  int64_t L = 1000000000;
  unsigned T;
  int64_t A, B;
  string s, center = "CENTER", hit = "HIT", miss = "MISS";
  bool center_hit = false;

  auto Search = [&](bool is_x, bool is_01, int64_t l, int64_t h, int64_t o) {
    if (center_hit) return l;
    for (; l < h;) {
      int64_t m = (l + h) >> 1;
      cout << (is_x ? m : o) << " " << (is_x ? o : m) << endl;
      cin >> s;
      if (s == center) {
        center_hit = true;
        return m;
      } else if ((s == hit) == is_01) {
        h = m;
      } else {
        l = m + 1;
      }
    }
    return l;
  };

  cin >> T >> A >> B;
  for (unsigned it = 1; it <= T; ++it) {
    center_hit = false;
    bool first_hit = false;
    int64_t x_fh, y_fh;
    for (int64_t x = -L / 2; (x < L) && !first_hit; x += L / 2) {
      for (int64_t y = -L / 2; (y < L) && !first_hit; y += L / 2) {
        cout << x << " " << y << endl;
        cin >> s;
        if ((s == center) || (s == hit)) {
          first_hit = true;
          x_fh = x;
          y_fh = y;
          center_hit = (s == center);
        }
      }
    }
    if (!first_hit) return -1;
    int64_t xl = Search(true, true, -L, x_fh, y_fh),
            xh = Search(true, false, x_fh, L + 1, y_fh),
            yl = Search(false, true, -L, y_fh, x_fh),
            yh = Search(false, false, y_fh, L + 1, x_fh);
    if (!center_hit) {
      cout << (xl + xh - 1) / 2 << " " << (yl + yh - 1) / 2 << endl;
      cin >> s;
      assert(s == center);
    }
  }
  return 0;
}
