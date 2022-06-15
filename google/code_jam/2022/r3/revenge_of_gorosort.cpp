#include "common/permutation/permutation.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/write.h"

#include <functional>

// Solution under 10k.
int main_revenge_of_gorosort() {
  unsigned T, N, K, R;
  cin >> T >> N >> K;
  vector<unsigned> vc(N);
  for (unsigned it = 1; it <= T;) {
    auto vi = nvector::Read<unsigned>(N);
    permutation::Permutation p(vi, true);
    fill(vc.begin(), vc.end(), 0);
    unsigned g = 0;

    function<void(const vector<unsigned>&)> Apply =
        [&](const vector<unsigned>& v) -> void {
      unsigned s = v.size();
      if (s < 6) {
        ++g;
        for (auto u : v) vc[u] = g;
      } else if (s == 6) {
        for (unsigned i = 0; i < s; ++i) vc[v[i]] = g + 1 + (i / 3);
        g += 2;
      } else if (((s >= 11) && (s <= 14)) || ((s >= 27) && (s <= 33))) {
        // Split 3 (about .2 * T operations saved against Split 2 only)
        auto l1 = s / 3;
        auto l2 = (2 * s) / 3;
        vc[v[0]] = vc[v[l1]] = vc[v[l2]] = ++g;
        Apply({v.begin() + 1, v.begin() + l1});
        Apply({v.begin() + l1 + 1, v.begin() + l2});
        Apply({v.begin() + l2 + 1, v.end()});
      } else {
        // Split 2
        auto l = s / 2;
        vc[v[0]] = vc[v[l]] = ++g;
        Apply({v.begin() + 1, v.begin() + l});
        Apply({v.begin() + l + 1, v.end()});
      }
    };

    for (auto& c : p.Cycles()) Apply(c);

    for (unsigned i = 0; i < N; ++i) {
      if (vc[i] == 0) vc[i] = ++g;
    }
    nvector::Write(vc);
    cin >> R;
    if (R == 1) ++it;
  }
  return 0;
}
