#include "common/geometry/d2/base.h"
#include "common/geometry/d2/iangle.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/stl_hash/iangle.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <unordered_map>
#include <unordered_set>

int main_wormhole_in_one() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    auto v = ReadVector<I2Point>(N);
    unordered_map<I2Angle, unordered_map<int64_t, unordered_set<unsigned>>> m;
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        if (i == j) continue;
        auto a = I2Angle(v[j] - v[i]);
        auto c = I2LinePV(v[i], a.ToVector())({});
        m[a][c].insert({i, j});
      }
    }
    unsigned best = 1;
    for (auto& p : m) {
      unsigned s = 0;
      for (auto& pp : p.second) s += pp.second.size();
      best = max(best, 2 + s - (s & 1));
    }
    cout << "Case #" << it << ": " << min(best, N) << endl;
  }
  return 0;
}
