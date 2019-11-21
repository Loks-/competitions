// https://www.hackerrank.com/challenges/hyperrectangle-gcd

#include "common/factorization/primes_list_extended/eulers_totient.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_hyperrectangle_gcd__totient() {
  factorization::ple::EulersTotient primes_list(100000);
  primes_list.Precalc();
  unsigned T, K;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> K;
    vector<unsigned> vnk = ReadVector<unsigned>(K);
    unsigned n0 = *min_element(vnk.begin(), vnk.end());
    TModularD s = 0;
    for (unsigned i = 1; i <= n0; ++i) {
      TModularD t = primes_list(i);
      for (unsigned nk : vnk) t *= TModularD(nk / i);
      s += t;
    }
    cout << s << endl;
  }
  return 0;
}
