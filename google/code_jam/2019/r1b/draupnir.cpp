#include "common/stl/base.h"
#include "common/vector/write.h"

int main_draupnir() {
  unsigned T, W;
  cin >> T >> W;
  vector<uint64_t> v(6);
  uint64_t r;
  for (unsigned it = 1; it <= T; ++it) {
    cout << 210 << endl;
    cin >> r;
    v[5] = (r >> 35) & 127;
    v[4] = (r >> 42) & 127;
    v[3] = (r >> 52) & 127;
    cout << 42 << endl;
    cin >> r;
    r -= (v[3] << 10) + (v[4] << 8) + (v[5] << 7);
    v[2] = (r >> 14) & 127;
    v[1] = (r >> 21) & 127;
    v[0] = (r >> 42) & 127;
    nvector::Write(v);
    cin >> r;
  }
  return 0;
}
