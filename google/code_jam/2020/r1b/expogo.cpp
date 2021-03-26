#include "common/stl/base.h"

int main_expogo() {
  vector<char> vx{'W', 'E'}, vy{'S', 'N'};
  vector<int64_t> vd{1, -1};
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int64_t x, y;
    cin >> x >> y;
    if ((x + y) & 1) {
      cout << "Case #" << it << ": ";
      for (; x || y; x /= 2, y /= 2) {
        bool final_step = (abs(x) + abs(y) == 1);
        bool next_bit = (((x + y) & 2) == 2);
        unsigned i = ((final_step ^ next_bit) ? 1 : 0);
        if (x & 1) {
          x += vd[i];
          cout << vx[i];
        } else {
          y += vd[i];
          cout << vy[i];
        }
      }
      cout << endl;
    } else {
      cout << "Case #" << it << ": "
           << "IMPOSSIBLE" << endl;
    }
  }
  return 0;
}
