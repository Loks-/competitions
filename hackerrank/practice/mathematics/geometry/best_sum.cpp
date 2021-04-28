// https://www.hackerrank.com/challenges/best-sum

#include "common/geometry/d2/angle_int.h"
#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/vector.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_best_sum() {
  unsigned T, n;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> n;
    auto vx = nvector::Read<int64_t>(n), vy = nvector::Read<int64_t>(n);
    std::vector<I2Vector> vv;
    for (unsigned i = 0; i < n; ++i) {
      I2Vector v(vx[i], vy[i]);
      if (!v.Empty()) vv.push_back(v);
    }
    n = vv.size();
    sort(vv.begin(), vv.end(),
         [](auto& l, auto& r) { return CompareVectorAngle(l, r); });
    int64_t m = 0;
    I2Vector vs;
    for (unsigned i = 0, j = 0; i < n; ++i) {
      m = max(m, vs.LengthSquared());
      auto ai = I2Angle(vv[i]);
      for (; (j < i + n); ++j) {
        auto vj = vv[j % n];
        auto r = vv[i] % vj;
        if ((r < 0) || ((r == 0) && (I2Angle(-vj) == ai))) break;
        vs += vj;
        m = max(m, vs.LengthSquared());
      }
      vs -= vv[i];
      m = max(m, vs.LengthSquared());
    }
    cout << m << endl;
  }
  return 0;
}
