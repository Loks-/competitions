#include "common/stl/base.h"
#include "common/vector/read.h"

int main_chain_reactions() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    auto vf = nvector::Read<uint64_t>(n);
    auto vl = nvector::Read<unsigned>(n);
    vector<uint64_t> vp(n, 0);
    uint64_t tf = 0;
    for (unsigned i = n - 1; i < n; --i) {
      vf[i] = max(vf[i], vp[i]);
      if (vl[i] == 0) {
        tf += vf[i];
      } else {
        auto j = vl[i] - 1;
        if (vp[j] == 0) {
          vp[j] = vf[i];
        } else {
          auto x1 = min(vp[j], vf[i]), x2 = max(vp[j], vf[i]);
          tf += x2;
          vp[j] = x1;
        }
      }
    }
    cout << "Case #" << it << ": " << tf << endl;
  }
  return 0;
}
