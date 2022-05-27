#include "common/stl/base.h"
#include "common/vector/read.h"

int main_squary() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    auto vs = nvector::Read<int64_t>(N);
    int64_t s1 = 0, s2 = 0;
    for (auto v : vs) {
      s1 += v;
      s2 += v * v;
    }
    auto d = s2 - s1 * s1;
    cout << "Case #" << it << ": ";
    if ((s1 == 0) && (s2 == 0)) {
      cout << 0 << endl;
    } else if ((s1 != 0) && ((d % (2 * s1)) == 0)) {
      cout << d / (2 * s1) << endl;
    } else if (K == 1) {
      cout << "IMPOSSIBLE" << endl;
    } else {
      s2 += (1 - s1) * (1 - s1);
      cout << (1 - s1) << " " << (s2 - 1) / 2 << endl;
    }
  }
  return 0;
}
