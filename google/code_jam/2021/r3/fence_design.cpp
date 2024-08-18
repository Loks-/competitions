#include "common/geometry/d2/hash/point.h"
#include "common/geometry/d2/line_pv.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/triangulation/base.h"
#include "common/hash.h"
#include "common/numeric/utils/sign.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <unordered_map>

int main_fence_design() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    auto vp = nvector::Read<I2Point>(N);
    auto vis = nvector::Read<unsigned>(4);
    unordered_map<I2Point, unsigned, DHash<I2Point>> mp2i;
    for (unsigned i = 0; i < N; ++i) mp2i[vp[i]] = i + 1;

    I2OpenSegment s1(vp[vis[0] - 1], vp[vis[1] - 1]),
        s2(vp[vis[2] - 1], vp[vis[3] - 1]);
    I2LinePV l1(s1.p1, s1.p2), l2(s2.p1, s2.p2);
    if (Sign(l1(s2.p1)) != Sign(l1(s2.p2))) {
      swap(l1, l2);
      swap(s1, s2);
    }
    auto s1i = s1.Flip(), s2i = s2.Flip();
    if ((l1(s2.p1) < 0) || (l1(s2.p2) < 0)) l1.SetOppositeNormal();
    vector<I2Point> vp1{s1.p1, s1.p2}, vp2{s2.p1, s2.p2}, vp3;
    for (auto& p : vp) {
      ((l1(p) < 0) ? vp1 : (l2(p) < 0) ? vp2 : vp3).push_back(p);
    }
    geometry::d2::triangulation::Base<int64_t> t1(vp1), t2(vp2), t3(vp3),
        t23(t2, t3, s2), tf(t1, t23, s1);

    cout << "Case #" << it << ": " << tf.GetSegments().size() - 2 << endl;
    for (auto& s : tf.GetSegments()) {
      if ((s == s1) || (s == s1i) || (s == s2) || (s == s2i)) continue;
      cout << mp2i[s.p1] << " " << mp2i[s.p2] << endl;
    }
  }
  return 0;
}
