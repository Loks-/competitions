// https://www.hackerrank.com/challenges/binomial-coefficients

#include "common/numeric/long/unsigned_io.h"
#include "common/stl/base.h"

int main_binomial_coefficients() {
  unsigned T, P;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    LongUnsigned n = numeric::nlong::UnsignedReadBase(), t = n, r(1u);
    cin >> P;
    for (; !t.Empty(); t /= P) {
      unsigned k = t % P;
      r *= (1 + k);
    }
    cout << (n + 1 - r) << endl;
  }
  return 0;
}
