// https://www.hackerrank.com/challenges/gcd-mocktail

#include "common/local.h"
#include "common/modular/static/sum_of_powers.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<30000001>;

int main_gcd_mocktail() {
  // Precalc sums
  // O(maxl * precalc_n) ~ 10^6
  unsigned maxl = local_run ? 10 : 100, precalc_n = local_run ? 100 : 10000;
  vector<vector<TModular>> vcache(maxl + 1, vector<TModular>(precalc_n + 1, 1));
  vcache[0][0] = 0;
  for (unsigned l = 1; l <= maxl; ++l) {
    for (unsigned i = 0; i <= precalc_n; ++i)
      vcache[l][i] = vcache[l - 1][i] * TModular(i);
  }
  for (unsigned l = 0; l <= maxl; ++l) {
    for (unsigned i = 1; i <= precalc_n; ++i) vcache[l][i] += vcache[l][i - 1];
  }

  ModularSumOfPowers<TModular> sum_of_powers;
  unsigned T, N, D, L, Q;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> N >> D >> Q;
    vector<unsigned> vk;
    vector<TModular> vc;
    // O(T * (N + 2 * sqrt(N) log D) ) ~ 10^8
    for (unsigned k = 1; k <= N + 1; ++k) {
      if ((k > 1) && (N / k == N / (k - 1))) continue;
      vk.push_back(k);
      vc.push_back(TModular(N / k).PowU(D));
    }
    // O(T * N) ~ 10^8
    for (size_t i = vk.size(); i--;) {
      unsigned k = vk[i], m = 1, mnext;
      for (size_t j = i + 1; j + 1 < vk.size(); ++j) {
        mnext = (vk[j + 1] - 1) / k;
        if (mnext == m) continue;
        vc[i] -= TModular(mnext - m) * vc[j];
        m = mnext;
      }
    }
    for (size_t i = 1; i < vc.size(); ++i) vc[i - 1] -= vc[i];
    // O(T * Q * sqrt(N) * L) ~ 10^8
    for (unsigned iQ = 0; iQ < Q; ++iQ) {
      cin >> L;
      const vector<TModular>& vlcache = vcache[L];
      TModular r = 0;
      for (unsigned i = 0; i + 1 < vk.size(); ++i) {
        unsigned k = vk[i + 1] - 1;
        r += vc[i] * (k <= precalc_n ? vlcache[k] : sum_of_powers.Sum(k, L));
      }
      cout << r << endl;
    }
  }
  return 0;
}
