// https://www.hackerrank.com/challenges/polygon

#include "common/geometry/d2/apolygon.h"
#include "common/geometry/d2/point_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_polygon() {
  unsigned N, Q, M;
  cin >> N >> Q;
  auto vp = nvector::Read<I2Point>(N);
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    cin >> M;
    auto vq = nvector::Read<I2Point>(M);
    auto ap = I2APolygon(vq);
    unsigned k = 0;
    for (auto& p : vp) {
      if (ap.Inside(p)) ++k;
    }
    cout << k << endl;
  }
  return 0;
}
