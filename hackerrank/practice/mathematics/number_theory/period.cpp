// https://www.hackerrank.com/challenges/period

#include "common/modular/utils/multiplicative_order.h"
#include "common/modular_proxy.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

using TModularProxy = TModularProxy_C32U;

int main_period() {
  class AC {
   public:
    uint64_t a, b;
    TModularProxy p;

    AC(uint64_t _a, uint64_t _b, const TModularProxy& _p) : p(_p) {
      a = p.ApplyU(_a);
      b = p.ApplyU(_b);
    }

    AC operator*(const AC& r) const {
      return AC(a * r.a + 5 * b * r.b, a * r.b + b * r.a, p);
    }

    AC& operator*=(const AC& r) {
      AC t = *this * r;
      *this = t;
      return *this;
    }
  };

  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    uint64_t a, b, m;
    cin >> a >> b >> m;
    TModularProxy p(m);
    AC x(a, b, p), xc(a, m - b, p), xxc = x * xc;
    uint64_t xmgcd = GCD(xxc.a, m);
    if (xmgcd != 1) {
      cout << -1 << endl;
      continue;
    }
    uint64_t cp = 1;
    AC xp = x;
    for (; xp.b; xp *= x) ++cp;
    cp *= GetMultiplicativeOrder(m, xp.a);
    cout << cp << endl;
  }
  return 0;
}
