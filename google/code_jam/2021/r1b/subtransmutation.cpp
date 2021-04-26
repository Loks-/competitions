#include "common/algorithm/linear_search.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_subtransmutation() {
  unsigned T, N, A, B;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> A >> B;
    auto v = nvector::Read<unsigned>(N);
    auto g = GCD(A, B), g1 = (N - 1) % g;
    std::vector<unsigned> vt;
    bool ok = true;
    for (unsigned i = 0; i < N; ++i) {
      if (!v[i]) continue;
      if ((i % g) != g1) ok = false;
    }

    auto F = [&](unsigned n) {
      vt.clear();
      vt.resize(++n, 0);
      vt.back() = 1;
      for (unsigned i = n; i-- > 0;) {
        auto v2i = (i < v.size() ? v[i] : 0);
        if (vt[i] < v2i) return false;
        if (i >= B) vt[i - B] += vt[i] - v2i;
        if (i >= A) vt[i - A] += vt[i] - v2i;
      }
      return true;
    };

    cout << "Case #" << it << ": "
         << (ok ? to_string(FirstIfB(N, F) + 1) : "IMPOSSIBLE") << endl;
  }
  return 0;
}
