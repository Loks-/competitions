#include "common/geometry/d2/base.h"
#include "common/geometry/d2/iangle.h"
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
    auto v = nvector::Read<I2Point>(N);
    unordered_map<I2Angle, unordered_set<unsigned>> m;
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        if (i == j) continue;
        m[I2Angle(v[j] - v[i])].insert({i, j});
      }
    }
    unsigned best = 0;
    for (auto& p : m) best = max(best, unsigned(p.second.size()));
    cout << "Case #" << it << ": " << min(best - (best & 1) + 2, N) << endl;
  }
  return 0;
}
