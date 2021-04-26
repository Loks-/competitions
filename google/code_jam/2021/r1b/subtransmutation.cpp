#include "common/binary_search.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/write.h"

#include <string>

int main_subtransmutation() {
  unsigned T, N, A, B, k;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> A >> B;
    auto v = nvector::Read<unsigned>(N);
    auto g = GCD(A, B), g1 = (N - 1) % g;
    A /= g;
    B /= g;
    std::vector<unsigned> v2((N - 1) / g + 1, 0), vt;
    bool ok = true;
    for (unsigned i = 0; i < N; ++i) {
      if (!v[i]) continue;
      if ((i % g) != g1) ok = false;
      v2[i / g] = v[i];
    }
    // nvector::Write(v2);

    auto F = [&](unsigned n) {
      vt.clear();
      vt.resize(++n, 0);
      vt.back() = 1;
      for (unsigned i = n; i-- > 0;) {
        auto v2i = (i < v2.size() ? v2[i] : 0);
        if (vt[i] < v2i) return false;
        if (i >= B) vt[i - B] += vt[i] - v2i;
        if (i >= A) vt[i - A] += vt[i] - v2i;
      }
      return true;
    };

    if (ok) {
      // k = LowerBoundB(unsigned(v2.size()), F);
      // for (unsigned i = max<unsigned>(v2.size() + A, k) - A; i < k; ++i) {
      for (unsigned i = v2.size();; ++i) {
        if (F(i)) {
          k = i;
          break;
        }
      }
    }
    cout << "Case #" << it << ": "
         << (ok ? to_string(k * g + g1 + 1) : "IMPOSSIBLE") << endl;
  }
  return 0;
}
