#include "common/modular_io.h"
#include "common/numeric/fibonacci.h"
#include "common/stl/base.h"

#include <functional>
#include <unordered_map>

using TModular = TModular_C32<100000000>;

int main_366() {
  uint64_t n = 1000000000000000000ull;
  numeric::Fibonacci fb;
  fb.AdjustMaxValue(n);
  vector<uint64_t> vf = fb.GetVector();

  auto S = [](uint64_t k) {
    return (k & 1) ? TModular(k) * TModular((k + 1) / 2)
                   : TModular(k / 2) * TModular(k + 1);
  };

  unordered_map<uint64_t, TModular> me;
  me[0] = 0;
  std::function<TModular(uint64_t)> E = [&](uint64_t n) -> TModular {
    auto itm = me.find(n);
    if (itm != me.end()) return itm->second;
    auto itf = upper_bound(vf.begin(), vf.end(), n);
    uint64_t i = itf - vf.begin() - 1, f = vf[i];
    uint64_t l = (f - 1) / 2, k = n - f;
    if (k <= l)
      return me[n] = E(f - 1) + S(k);
    else
      return me[n] = E(f - 1) + S(l) + E(n - f) - E(l);
  };

  cout << E(n) << endl;
  return 0;
}
