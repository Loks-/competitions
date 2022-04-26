#include "common/numeric/utils/abs.h"
#include "common/stl/base.h"
#include "common/vector/max.h"
#include "common/vector/min.h"
#include "common/vector/read.h"

int main_controlled_inflation() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n, p;
    cin >> n >> p;
    int64_t v0 = 0, v1 = 0, s0 = 0, s1 = 0;
    for (unsigned i = 0; i < n; ++i) {
      auto v = nvector::Read<int64_t>(p);
      auto nv0 = nvector::Min(v), nv1 = nvector::Max(v);
      auto ns0 = (nv1 - nv0) + min(s0 + Abs(v0 - nv1), s1 + Abs(v1 - nv1)),
           ns1 = (nv1 - nv0) + min(s0 + Abs(v0 - nv0), s1 + Abs(v1 - nv0));
      v0 = nv0;
      v1 = nv1;
      s0 = ns0;
      s1 = ns1;
    }
    cout << "Case #" << it << ": " << min(s0, s1) << endl;
  }
  return 0;
}
