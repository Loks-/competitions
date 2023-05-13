#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_illumination_optimization() {
  unsigned T, N;
  int64_t M, R;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> M >> R >> N;
    auto vx = nvector::Read<int64_t>(N);
    vx.insert(vx.begin(), -R);
    vx.push_back(M + R);
    bool ok = true;
    int64_t xl = 0;
    unsigned cnt = 0;
    for (unsigned i = 1; i < vx.size(); ++i) {
      if (vx[i] - vx[i - 1] > 2 * R) ok = false;
      if (vx[i] > xl + R) {
        ++cnt;
        xl = vx[i - 1] + R;
      }
    }
    cout << "Case #" << it << ": " << (ok ? to_string(cnt) : "IMPOSSIBLE")
         << std::endl;
  }
  return 0;
}
