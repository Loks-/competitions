#include "common/stl/base.h"
#include "common/vector/make.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

int main_weightlifting() {
  unsigned T, E, W;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> E >> W;
    nvector::DVector<unsigned, 2> vv(E);
    auto vr = nvector::Make<unsigned>(E, E + 1, -1u);
    for (unsigned e = 0; e < E; ++e) {
      vv[e] = nvector::Read<unsigned>(W);
      vr[e][e + 1] = nvector::Sum(vv[e]);
      for (unsigned e1 = e - 1; e1 < E; --e1) {
        for (unsigned w = 0; w < W; ++w) vv[e1][w] = min(vv[e][w], vv[e1][w]);
        auto smx = nvector::Sum(vv[e1]);
        for (unsigned e2 = e1 + 1; e2 <= e; ++e2)
          vr[e1][e + 1] = min(vr[e1][e + 1], vr[e1][e2] + vr[e2][e + 1] - smx);
      }
    }
    cout << "Case #" << it << ": " << 2 * vr[0][E] << endl;
  }
  return 0;
}
