// https://www.hackerrank.com/challenges/permutation-problem

#include "common/local.h"
#include "common/modular/static/factorial.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TFactorial = modular::mstatic::Factorial<TModularD>;

int main_permutation_problem() {
  const uint64_t M = TModularD::GetMod();
  TFactorial f;
  unsigned maxd = 11, maxn = local_run ? 10 : 1001;
  vector<vector<vector<TModularD>>> v(
      maxd, vector<vector<TModularD>>(maxn, vector<TModularD>(maxn)));
  v[0][0][0] = f.Get(9) * 9;  // 9/10 * 10!
  for (unsigned d = 1; d < maxd; ++d) {
    TModularD vd0 = v[0][0][0] * f.GetI(d);
    for (unsigned k = 0; k < maxn; ++k) v[d][0][k] = vd0;
    for (unsigned n = 1; n < maxn; ++n) {
      for (unsigned k = 1; k <= n; ++k) {
        TModularD s = v[d][n][k - 1], fik = f.GetI(k), fikpe = fik;
        unsigned nek = n;
        for (unsigned e = 1; e <= d; ++e) {
          if (k > nek) break;
          nek -= k;
          s += f.GetI(e) * fikpe * v[d - e][nek][min(nek, k - 1)];
          fikpe *= fik;
        }
        v[d][n][k] = s;
      }
    }
  }

  unsigned T, n, k;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> n >> k;
    cout << v[10][n][min(n, k)] * f.Get(n) << endl;
  }
  return 0;
}
