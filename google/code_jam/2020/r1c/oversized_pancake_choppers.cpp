#include "common/numeric/fraction.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <map>

using TFraction = numeric::Fraction<uint64_t>;

int main_oversized_pancake_choppers() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    uint64_t N, D;
    cin >> N >> D;
    auto v = nvector::Read<uint64_t>(N);
    vector<TFraction> vf;
    map<TFraction, vector<uint64_t>> m;
    for (auto vv : v) {
      for (uint64_t k = 1; k <= D; ++k) {
        TFraction f(vv, k);
        vf.push_back(f);
        m[f].push_back(k);
      }
    }
    sort(vf.begin(), vf.end());
    vf.push_back(vf.back() * N);
    auto fend = *lower_bound(vf.begin(), vf.end(), TFraction(0),
                             [&](const TFraction& l, const TFraction&) {
                               uint64_t d = 0;
                               for (auto vv : v) {
                                 d += (vv * l.GetD()) / l.GetN();
                               }
                               return d >= D;
                             });
    uint64_t best = 1;
    for (auto p : m) {
      if (p.first >= fend) continue;
      auto& vp = p.second;
      if (vp.size() <= best) continue;
      sort(vp.begin(), vp.end());
      uint64_t cs = 0, cd = 0;
      for (auto vv : vp) {
        if (cd + vv > D) break;
        cs += 1;
        cd += vv;
      }
      if (best < cs) best = cs;
    }
    cout << "Case #" << it << ": " << D - best << endl;
  }
  return 0;
}
