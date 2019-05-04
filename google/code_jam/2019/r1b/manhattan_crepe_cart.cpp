#include "common/stl/base.h"

int main_manhattan_crepe_cart() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned p, q;
    cin >> p >> q;
    vector<int> vx(q + 1, 0), vy(q + 1, 0);
    for (unsigned i = 0; i < p; ++i) {
      unsigned x, y;
      char c;
      cin >> x >> y >> c;
      switch (c) {
        case 'W':
          vx[x] -= 1;
          break;
        case 'E':
          vx[x + 1] += 1;
          break;
        case 'S':
          vy[y] -= 1;
          break;
        case 'N':
          vy[y + 1] += 1;
          break;
      }
    }
    unsigned bx = 0, by = 0;
    for (unsigned i = 1; i <= q; ++i) {
      vx[i] += vx[i - 1];
      vy[i] += vy[i - 1];
      if (vx[i] > vx[bx]) bx = i;
      if (vy[i] > vy[by]) by = i;
    }
    cout << "Case #" << it << ": " << bx << " " << by << std::endl;
  }
  return 0;
}
