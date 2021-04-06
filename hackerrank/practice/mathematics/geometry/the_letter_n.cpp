// https://www.hackerrank.com/challenges/n-letter

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/compare/vector_angle.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/vector.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_the_letter_n() {
  auto cmp = CompareVectorAngle<int64_t>;
  unsigned N;
  cin >> N;
  auto vp = nvector::Read<I2Point>(N);
  vector<vector<uint64_t>> vc(N, vector<uint64_t>(N, 0));
  for (unsigned i = 0; i < N; ++i) {
    vector<I2Vector> vi;
    for (unsigned j = 0; j < N; ++j) {
      if (j == i) continue;
      vi.push_back(vp[j] - vp[i]);
    }
    sort(vi.begin(), vi.end(), cmp);
    for (unsigned j = 0; j < N; ++j) {
      if (j == i) continue;
      I2Vector v2 = vp[j] - vp[i], v1(v2.dy, -v2.dx);
      auto it1 = lower_bound(vi.begin(), vi.end(), v1, cmp),
           it2 = lower_bound(vi.begin(), vi.end(), v2, cmp);
      vc[i][j] = (it1 <= it2) ? (it2 - it1) : (vi.size() - (it1 - it2));
    }
  }
  uint64_t s = 0;
  for (unsigned i = 0; i < N; ++i) {
    for (unsigned j = i + 1; j < N; ++j) s += vc[i][j] * vc[j][i];
  }
  cout << s << endl;
  return 0;
}
