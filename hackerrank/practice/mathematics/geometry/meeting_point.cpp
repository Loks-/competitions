// https://www.hackerrank.com/challenges/meeting-point

#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/utils/rotate_pi4s_points.h"
#include "common/stl/base.h"
#include "common/vector/min.h"
#include "common/vector/read.h"

int main_meeting_point() {
  unsigned n;
  cin >> n;
  auto vp = nvector::Read<I2Point>(n);
  RotatePi4S(vp);
  vector<vector<pair<int64_t, unsigned>>> v(2);
  vector<int64_t> vs(n + 1, 0), vr(n, 0);
  for (unsigned i = 0; i < n; ++i) {
    v[0].push_back({vp[i].x, i});
    v[1].push_back({vp[i].y, i});
  }
  for (auto &vi : v) {
    sort(vi.begin(), vi.end());
    for (unsigned i = 0; i < n; ++i) vs[i + 1] = vs[i] + vi[i].first;
    for (unsigned i = 0; i < n; ++i)
      vr[vi[i].second] += vs[n] - 2 * vs[i] + vi[i].first * int(2 * i - n);
  }
  cout << nvector::Min(vr) / 2 << endl;
  return 0;
}
