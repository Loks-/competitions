#include "common/stl/base.h"
#include "common/vector/read.h"

#include <iomanip>

int main_indispensable_overpass() {
  cout << setprecision(6) << fixed;
  unsigned T, W, E, C;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> W >> E >> C;
    vector<unsigned> vw = nvector::Read<unsigned>(W - 1),
                     ve = nvector::Read<unsigned>(E - 1), vtsw(W + 1, 1),
                     vtse(E + 1, 1);
    vector<uint64_t> vdsw(W + 1, 0), vdse(E + 1, 0);
    uint64_t r0 = 0, WE = W + E, d = (WE * (WE - 1)) / 2;
    for (auto we : {true, false}) {
      auto& v = (we ? vw : ve);
      auto& vts = (we ? vtsw : vtse);
      auto& vds = (we ? vdsw : vdse);
      v.insert(v.begin(), 0u);
      for (unsigned i = 1; i < v.size(); ++i) {
        auto j = v[i];
        vts[j] += vts[i];
        vds[j] += vds[i] + vts[i];
      }
      for (unsigned i = v.size() - 1; i; --i)
        vds[i] = (we ? W : E) + vds[v[i]] - 2 * vts[i];
      for (auto& x : vds) r0 += x;
    }
    r0 = r0 / 2 + uint64_t(W) * uint64_t(E);
    cout << "Case #" << it << ":";
    for (unsigned ic = 0; ic < C; ++ic) {
      unsigned a, b;
      cin >> a >> b;
      cout << " " << double(r0 + vdsw[a] * E + vdse[b] * W) / double(d);
    }
    cout << endl;
  }
  return 0;
}
