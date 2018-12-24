// https://www.hackerrank.com/challenges/circles-math

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_circles_math() {
  TFactorial f;
  unsigned maxn = 1000001;
  vector<TModular> ve(maxn);
  TModular sone = 1;
  ve[0] = 1;
  for (unsigned i = 1; i < maxn; ++i) {
    sone = -sone;
    ve[i] = TModular(i) * ve[i - 1] + sone;
  }

  unsigned T, N, K;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    cin >> N >> K;
    if (K == 1) {
      cout << 0 << endl;
    } else {
      sone = 1;
      TModular s = 0, r = 1, ki = TModular(K).Inverse();
      for (unsigned l = 1; l * K <= N; ++l) {
        r *= ki;
        s +=
            sone * ve[N - l * K] * f.Get(N) * f.GetI(N - l * K) * f.GetI(l) * r;
        sone = -sone;
      }
      cout << s << endl;
    }
  }
  return 0;
}
