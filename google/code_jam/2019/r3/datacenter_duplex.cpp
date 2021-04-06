#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_datacenter_duplex() {
  unsigned T, R, C;
  cin >> T;
  for (unsigned iT = 1; iT <= T; ++iT) {
    cin >> R >> C;
    ds::DisjointSet djs(R * C);
    auto vs = nvector::Read<string>(R);
    for (unsigned i = 0; i < R; ++i) {
      for (unsigned j = 0; j < C; ++j) {
        if ((i + 1 < R) && (vs[i][j] == vs[i + 1][j]))
          djs.Union(i * C + j, i * C + j + C);
        if ((j + 1 < C) && (vs[i][j] == vs[i][j + 1]))
          djs.Union(i * C + j, i * C + j + 1);
      }
    }
    vector<string> vo(R - 1, string(C - 1, '.'));
    for (unsigned i = 0; i < R - 1; ++i) {
      for (unsigned j = 0; j < C - 1; ++j) {
        if ((vs[i][j] == vs[i + 1][j + 1]) &&
            (djs.Find(i * C + j) != djs.Find(i * C + j + C + 1))) {
          vo[i][j] = '\\';
          djs.Union(i * C + j, i * C + j + C + 1);
        } else if ((vs[i][j + 1] == vs[i + 1][j]) &&
                   (djs.Find(i * C + j + 1) != djs.Find(i * C + j + C))) {
          vo[i][j] = '/';
          djs.Union(i * C + j + 1, i * C + j + C);
        }
      }
    }
    if (djs.GetSetsCount() == 2) {
      cout << "Case #" << iT << ": "
           << "POSSIBLE" << endl;
      for (auto& s : vo) cout << s << endl;
    } else {
      cout << "Case #" << iT << ": "
           << "IMPOSSIBLE" << endl;
    }
  }
  return 0;
}
