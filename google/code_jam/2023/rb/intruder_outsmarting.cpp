#include "common/modular/arithmetic.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

using TModular = modular::TArithmetic_C32U;

int main_intruder_outsmarting() {
  unsigned T, W, N, D;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> W >> N >> D;
    auto g = GCD(N, D);
    N /= g;
    D /= g;
    auto v = nvector::Read<int64_t>(W);
    bool ok = true;
    uint64_t r = 0;
    for (unsigned i = 0; i < W / 2; ++i) {
      auto d = v[i] - v[W - i - 1];
      if ((d % g) != 0) {
        ok = false;
        break;
      }
      auto k = TModular::Div(TModular::ApplyS(d / g, N), D, N);
      r += min(k, N - k);
    }
    cout << "Case #" << it << ": " << (ok ? to_string(r) : "IMPOSSIBLE")
         << endl;
  }
  return 0;
}
