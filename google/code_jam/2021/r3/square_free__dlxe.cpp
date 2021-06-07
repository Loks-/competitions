#include "common/cover/dlxe.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <numeric>
#include <string>

// Small test only.
int main_square_free__dlxe() {
  unsigned T, R, C;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> C;
    auto vr = nvector::Read<unsigned>(R), vc = nvector::Read<unsigned>(C);
    if (accumulate(vr.begin(), vr.end(), 0u) !=
        accumulate(vc.begin(), vc.end(), 0u)) {
      cout << "Case #" << it << ": IMPOSSIBLE" << endl;
      continue;
    }
    unsigned rcmin = min(R, C), squares = 0;
    for (unsigned k = 2; k <= rcmin; k += 2)
      squares += (R - k + 1) * (C - k + 1);
    cover::DLXE dlx;
    dlx.Init(2 * R * C, R * C + R + C + squares);
    for (unsigned i = 0; i < R; ++i) {
      for (unsigned j = 0; j < C; ++j) {
        unsigned k = i * C + j;
        dlx.Add(2 * k, k);
        dlx.Add(2 * k + 1, k);
        dlx.Add(2 * k + 1, R * C + i);
        dlx.Add(2 * k + 1, R * C + R + j);
      }
    }
    vector<unsigned> vdlxc_min(R * C, 1);
    vdlxc_min.insert(vdlxc_min.end(), vr.begin(), vr.end());
    vdlxc_min.insert(vdlxc_min.end(), vc.begin(), vc.end());
    vector<unsigned> vdlxc_max = vdlxc_min;
    vdlxc_min.insert(vdlxc_min.end(), squares, 0);
    {
      unsigned jk = R * C + R + C;
      for (unsigned k = 2; k <= rcmin; k += 2) {
        for (unsigned i = 0; i + k <= R; ++i) {
          for (unsigned j = 0; j + k <= C; ++j) {
            vdlxc_max.push_back(2 * k - 1);
            for (unsigned l = 0; l < k / 2; ++l) {
              dlx.Add(2 * ((i + l) * C + j + (k / 2 - 1 - l)) + 1, jk);
              dlx.Add(2 * ((i + l) * C + j + (k / 2 + l)), jk);
              dlx.Add(2 * ((i + k - l - 1) * C + j + (k / 2 - 1 - l)), jk);
              dlx.Add(2 * ((i + k - l - 1) * C + j + (k / 2 + l)) + 1, jk);
            }
            ++jk;
          }
        }
      }
    }
    dlx.SetColumnsMinMax(vdlxc_min, vdlxc_max);
    bool ok = dlx.SearchAny();
    cout << "Case #" << it << ": " << (ok ? "POSSIBLE" : "IMPOSSIBLE") << endl;
    if (ok) {
      vector<string> vs(R, string(C, ' '));
      for (unsigned r : dlx.SelectedRows()) {
        vs[r / (2 * C)][(r / 2) % C] = (r & 1) ? '/' : '\\';
      }
      for (auto& s : vs) cout << s << endl;
    }
  }
  return 0;
}
