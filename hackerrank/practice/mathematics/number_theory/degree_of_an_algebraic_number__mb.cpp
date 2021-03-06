// https://www.hackerrank.com/challenges/degree-of-an-algebraic-number

#include "common/factorization/factorization.h"
#include "common/linear_algebra/bool/matrix.h"
#include "common/linear_algebra/bool/rank.h"
#include "common/modular/static/bool.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <set>

using TModular = ModularBool;
using TMatrix = la::MatrixBool;

int main_degree_of_an_algebraic_number__mb() {
  unsigned T, N;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    set<unsigned> ps;
    cin >> N;
    vector<unsigned> va = nvector::Read<unsigned>(N);
    vector<vector<unsigned>> vap;
    for (unsigned a : va) {
      TFactorization f = Factorize(a);
      vector<unsigned> vp;
      for (auto p : f) {
        if (p.power & 1) {
          ps.insert(unsigned(p.prime));
          vp.push_back(unsigned(p.prime));
        }
      }
      vap.push_back(vp);
    }
    vector<unsigned> pv(ps.begin(), ps.end());
    TMatrix m(N, unsigned(ps.size()));
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned p : vap[i]) {
        unsigned j =
            unsigned(lower_bound(pv.begin(), pv.end(), p) - pv.begin());
        m.Set(i, j, ModularBool(1));
      }
    }
    cout << PowU<uint64_t>(2, la::Rank(m)) << endl;
  }
  return 0;
}
