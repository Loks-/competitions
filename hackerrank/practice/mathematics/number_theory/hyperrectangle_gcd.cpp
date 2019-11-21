// https://www.hackerrank.com/challenges/hyperrectangle-gcd

#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_hyperrectangle_gcd() {
  vector<TModularD> v(100001);
  unsigned T, K;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> K;
    vector<unsigned> vnk = ReadVector<unsigned>(K);
    unsigned n0 = *min_element(vnk.begin(), vnk.end());
    for (unsigned i = 1; i <= n0; ++i) {
      v[i] = 1;
      for (unsigned nk : vnk) v[i] *= TModularD(nk / i);
    }
    TModularD s = 0;
    for (unsigned i = n0; i; --i) {
      for (unsigned j = 2 * i; j <= n0; j += i) v[i] -= v[j];
      s += v[i] * TModularD(i);
    }
    cout << s << endl;
  }
  return 0;
}
