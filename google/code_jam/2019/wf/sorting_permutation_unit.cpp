#include "common/permutation/inverse_line.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/rotate.h"
#include "common/vector/transform.h"

#include <queue>

int main_sorting_permutation_unit() {
  vector<unsigned> vrb{1, 3, 12, 20};
  unsigned T, P, S, K, N, R = vrb.size() + 1;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> P >> S >> K >> N;
    unsigned N1 = N - 1;
    vector<unsigned> vr1(N1, N);
    vr1[0] = 0;
    queue<unsigned> q;
    for (q.push(0); !q.empty(); q.pop()) {
      unsigned qr = q.front();
      for (unsigned i = 0; i < vrb.size(); ++i) {
        unsigned r0 = (qr + vrb[i]) % N1;
        if (vr1[r0] != N) continue;
        vr1[r0] = i + 1;
        q.push(r0);
      }
    }
    cout << "Case #" << it << ":" << endl << R << endl;
    for (unsigned r : vrb) {
      r %= N1;
      for (unsigned i = 0; i < r; ++i) cout << i + N1 - r + 1 << " ";
      for (unsigned i = r; i < N1; ++i) cout << i - r + 1 << " ";
      cout << N << endl;
    }
    cout << N << " ";
    for (unsigned i = 2; i < N; ++i) cout << i << " ";
    cout << 1 << endl;
    for (unsigned jt = 0; jt < K; ++jt) {
      auto vi = nvector::Read<unsigned>(N);
      auto vpp = nvector::TransformVI(
          vi, [](unsigned x, unsigned i) { return make_pair(x, i); });
      sort(vpp.begin(), vpp.end());
      auto vp1 = nvector::TransformV(vpp, [](auto& p) { return p.second; });
      auto vp = permutation::Inverse(vp1);
      vector<unsigned> vs;
      unsigned i0 = 0;

      auto Rotate = [&](unsigned i0_new) {
        assert(vp[i0] == 0);
        for (; i0 != i0_new;) {
          auto index = vr1[(N1 + i0_new - i0) % N1];
          assert((index > 0) && (index < N));
          vs.push_back(index);
          auto r = vrb[index - 1] % N1;
          nvector::SwapIntervals(vp.begin(), vp.end() - r - 1, vp.end() - 1);
          i0 = (i0 + r) % N1;
        }
        assert(vp[i0] == 0);
      };

      if (vp.back() == 0) {
        vs.push_back(R);
        swap(vp[0], vp.back());
      } else {
        for (; vp[i0];) ++i0;
      }
      for (;;) {
        if (vp.back() == N1) {
          bool stop = true;
          for (unsigned j = 1; j < N1; ++j) {
            if (vp[(i0 + j) % N1] != j) {
              stop = false;
              Rotate(N1 - j);
              vs.push_back(R);
              swap(vp[0], vp.back());
              break;
            }
          }
          if (stop) break;
        } else {
          Rotate(N1 - vp.back());
          vs.push_back(R);
          swap(vp[0], vp.back());
        }
      }
      Rotate(0);
      cout << vs.size();
      for (auto s : vs) cout << " " << s;
      cout << endl;
    }
  }
  return 0;
}
