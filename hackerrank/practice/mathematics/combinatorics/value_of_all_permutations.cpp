// https://www.hackerrank.com/challenges/value-of-all-permutations

#include "common/modular/proxy/factorial.h"
#include "common/modular_proxy.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_value_of_all_permutations() {
  unsigned p, n, q, l = 1, m;
  cin >> p >> n;
  TModularProxy_P32U mp(p);
  PModularFactorial<TModularProxy_P32U> f(p);
  vector<unsigned> v = ReadVector<unsigned>(n);
  sort(v.begin(), v.end());
  uint64_t total = f.Get(n);
  vector<uint64_t> vs(n);
  vs[0] = v[0];
  for (unsigned i = 1; i < v.size(); ++i) {
    vs[i] = vs[i - 1] + v[i];
    if (v[i] != v[i - 1]) {
      total = mp.Mult(total, f.GetI(l));
      l = 0;
    }
    ++l;
  }
  total = mp.Mult(total, f.GetI(l));
  cin >> q;
  for (unsigned iq = 0; iq < q; ++iq) {
    cin >> m;
    auto it = upper_bound(v.begin(), v.end(), m);
    unsigned l = (it - v.begin()) - 1;
    cout << ((m < v[0]) ? 0ull : mp.Div(mp.MultSafe(vs[l], total), n - l))
         << endl;
  }
  return 0;
}
