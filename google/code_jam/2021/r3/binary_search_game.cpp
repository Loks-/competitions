#include "common/modular_io.h"
#include "common/numeric/bits/bits_count.h"
#include "common/polynomial/utils/interpolate_vy.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_binary_search_game() {
  unsigned T, N, M, L;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> M >> L;
    TModularD m(M);
    unsigned P2L = (1u << L), N0 = 0;
    auto va = nvector::Read<unsigned>(P2L);
    vector<unsigned> vac(N, 0), vam2(N, N), vc2;
    for (auto& a : va) vac[--a]++;
    for (unsigned i = 0; i < N; ++i) {
      if (vac[i] > 1) {
        vam2[i] = vc2.size();
        vc2.push_back(i);
      } else if (vac[i] == 0) {
        ++N0;
      }
    }
    unsigned N2 = vc2.size(), P2N2 = (1u << N2);
    vector<TModularD> vr(1, 0), vc0(P2L), vc1(P2L);
    for (unsigned k = 1; k < N + 2; ++k) {
      TModularD r = 0, m0 = k - 1, m1 = m - m0;
      for (unsigned in = 0; in < P2N2; ++in) {
        for (unsigned i = 0; i < P2L; ++i) {
          auto c = va[i];
          if (vac[c] == 1) {
            vc0[i] = m0;
            vc1[i] = m1;
          } else {
            vc1[i] = ((1u << vam2[c]) & in) ? 1u : 0u;
            vc0[i] = TModularD(1) - vc1[i];
          }
        }
        for (unsigned l = 0; l < L; ++l) {
          unsigned p2l = (1u << l);
          for (unsigned i = 0; i < P2L; i += 2 * p2l) {
            auto r0 = vc0[i] * vc0[i + p2l], r1 = vc1[i] * vc1[i + p2l],
                 rq = vc0[i] * vc1[i + p2l] + vc1[i] * vc0[i + p2l];
            vc0[i] = r0;
            vc1[i] = r1;
            (((l + L) & 1) ? vc1 : vc0)[i] += rq;
          }
        }
        auto n1 = numeric::BitsCount(in);
        r += m0.PowU(N2 - n1) * m1.PowU(n1) * vc1[0];
      }
      vr.push_back(r + vr.back());
    }
    cout << "Case #" << it << ": "
         << polynomial::Interpolate(vr, m) * m.PowU(N0) << endl;
  }
  return 0;
}
