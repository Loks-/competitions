// https://www.hackerrank.com/challenges/colorful-polygon

#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TModular = Modular<>;

int main_colorful_polygon() {
  TModular two = 2;
  unsigned n, i, j, u = -1u;
  cin >> n;
  vector<unsigned> v = ReadVector<unsigned>(n);
  vector<unsigned> vlast(n, n), vlast_map(n + 1, u);
  unsigned best_i = 0, best_d = n, different_values = 0;
  for (unsigned i = 0; i < 2 * n; ++i) {
    unsigned in = i % n, j = v[in];
    if (vlast_map[j] != u) {
      vlast[in] = i - vlast_map[j];
      if (best_d > vlast[in]) {
        best_d = vlast[in];
        best_i = in;
      }
    } else {
      different_values += 1;
    }
    vlast_map[j] = i;
  }
  if (different_values == n) {
    cout << two.PowU(n) - (n + 1) << endl;
  } else if (different_values == n - 1) {
    cout << two.PowU(n) - two.PowU(best_d - 1) - two.PowU(n - best_d - 1) - 1
         << endl;
  } else {
    TModular s = 0;
    unsigned bis = best_i + n - vlast[best_i];
    vector<TModular> vc(n), vcs(n);
    for (unsigned is = bis; is <= best_i + n; ++is) {
      unsigned l = is;
      fill(vc.begin(), vc.end(), 0);
      fill(vcs.begin(), vcs.end(), 0);
      vcs[is % n] = vc[is % n] = 1;
      for (unsigned i = is + 1; i < is + n; ++i) {
        vc[i % n] = vcs[(i - 1) % n] - vcs[(l - 1) % n];
        vcs[i % n] = vcs[(i - 1) % n] + vc[i % n];
        l = max(l, i - vlast[i % n]);
      }
      s += vcs[(bis - 1) % n] - vcs[(l - 1) % n];
    }
    cout << s << endl;
  }
  return 0;
}
