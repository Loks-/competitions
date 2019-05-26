// https://www.hackerrank.com/challenges/sasha-and-swaps

#include "common/numeric/utils/gcd.h"
#include "common/permutation/base.h"
#include "common/permutation/fixed_point.h"
#include "common/permutation/permutation.h"
#include "common/permutation/root.h"
#include "common/permutation/simple_cycle_pow.h"
#include "common/permutation/swaps.h"
#include "common/stl/base.h"

using namespace permutation;

int main_sasha_and_swaps() {
  unsigned N, K, T;
  cin >> N >> K >> T;
  Permutation p;
  p.ReadAsLine(N, true);
  auto proot = Root(p, T);
  bool ok = (proot.Size() == N);
  auto swaps = Swaps(proot);
  unsigned min_k = swaps.size();
  if (min_k > K) ok = false;
  if (ok && ((K - min_k) & 1)) {
    if (T & 1) {
      ok = false;
    } else {
      vector<unsigned> v = FixedPoints(proot.Line());
      bool fixed = false;
      if (v.size() >= 2) {
        TCycle new_cycle{v[0], v[1]};
        auto cycles = proot.Cycles();
        cycles.push_back(new_cycle);
        proot.SetCycles(cycles);
        fixed = true;
      } else {
        vector<vector<TCycle>> cycles_by_size(N + 1);
        for (const auto& cycle : proot.Cycles())
          cycles_by_size[cycle.size()].push_back(cycle);
        for (unsigned i = 3; i <= N; i += 2) {
          if ((GCD(i, T) != 1) || (cycles_by_size[i].size() < 2)) continue;
          auto c1 = cycles_by_size[i][0];
          auto c2 = cycles_by_size[i][1];
          SimpleCyclePow(c1, 2);
          SimpleCyclePow(c2, 2);
          TCycle new_cycle;
          for (unsigned j = 0; j < 2 * i; ++j)
            new_cycle.push_back(((j & 1) ? c1 : c2)[j / 2]);
          TCycles new_cycles;
          for (unsigned j = 0; j <= N; ++j) {
            for (unsigned k = (j == i) ? 2 : 0; k < cycles_by_size[j].size();
                 ++k)
              new_cycles.push_back(cycles_by_size[j][k]);
          }
          new_cycles.push_back(new_cycle);
          proot.SetCycles(new_cycles);
          fixed = true;
          break;
        }
      }
      if (fixed) {
        swaps = Swaps(proot);
        min_k = swaps.size();
      } else {
        ok = false;
      }
    }
  }
  if (ok) {
    assert((K >= min_k) && (((K - min_k) & 1) == 0));
    for (unsigned i = min_k; i < K; ++i) cout << "1 2" << endl;
    for (auto& s : swaps) cout << s.first + 1 << " " << s.second + 1 << endl;
  } else {
    cout << "no solution" << endl;
  }
  return 0;
}
