#include "common/stl/base.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/intersect_segment.h"
#include "common/vector/read.h"

int main_fence_design__small() {
  unsigned T, N, sp1, sp2;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    auto vp = nvector::Read<I2Point>(N);
    vector<I2OpenSegment> vs;
    vector<pair<unsigned, unsigned>> vsp;
    for (unsigned i = 0; i < 2; ++i) {
      cin >> sp1 >> sp2;
      vs.push_back(I2OpenSegment(vp[sp1 - 1], vp[sp2 - 1]));
    }
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = i + 1; j < N; ++j) {
        I2OpenSegment s(vp[i], vp[j]);
        bool good = true;
        for (auto& is : vs) {
          if (Intersect(s, is)) {
            good = false;
            break;
          }
        }
        if (good) { vs.push_back(s); vsp.push_back({i + 1, j + 1}); }
      }
    }
    cout << "Case #" << it << ": " << vsp.size() << endl;
    for (auto p : vsp) cout << p.first << " " << p.second << endl;
  }
  return 0;
}
