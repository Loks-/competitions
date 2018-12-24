#include "common/stl/base.h"

int main_go_gopher() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    int A, dx, dy, lx, ly;
    cin >> A;
    if (A == 20) {
      dx = 5;
      dy = 4;
    } else if (A == 200) {
      dx = 20;
      dy = 10;
    }
    cout << 500 << " " << 500 << endl;
    cin >> lx >> ly;
    int sx = lx, sy = ly, x = 0, y = 0;
    vector<vector<bool>> visited(dx, vector<bool>(dy, false));
    for (; (lx > 0) && (ly > 0);) {
      visited[lx - sx][ly - sy] = true;
      for (; visited[x][y];) {
        ++x;
        if (x == dx) {
          x = 0;
          ++y;
        }
      }
      cout << sx + min(x + 1, dx - 2) << " " << sy + min(y + 1, dy - 2) << endl;
      cin >> lx >> ly;
    }
  }
  return 0;
}
