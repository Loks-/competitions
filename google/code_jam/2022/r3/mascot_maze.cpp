#include "common/graph/graph.h"
#include "common/graph/graph/degeneracy_order.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_mascot_maze() {
  unsigned T, N, M = 13;
  string s = "ACDEHIJKMORST";
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N;
    auto vl = nvector::Read<unsigned>(N), vr = nvector::Read<unsigned>(N);
    bool ok = true;
    UndirectedGraph g(N);
    for (unsigned i = 0; i < N; ++i) {
      for (auto j : {vl[i], vr[i]}) {
        --j;
        if (j == i) ok = false;
        g.AddEdge(i, j);
        for (auto k : {vl[j], vr[j]}) {
          --k;
          if (k == i) ok = false;
          g.AddEdge(i, k);
        }
      }
    }
    cout << "Case #" << it << ": ";
    if (!ok) {
      cout << "IMPOSSIBLE" << endl;
      continue;
    }
    auto vo = DegeneracyOrder(g);
    reverse(vo.begin(), vo.end());
    vector<unsigned> vs(N, M);
    for (auto i : vo) {
      for (unsigned j = 0; j < M; ++j) {
        unsigned k = (i + j) % M;
        ok = true;
        for (auto v : g.Edges(i)) {
          if (vs[v] == k) {
            ok = false;
            break;
          }
        }
        if (ok) {
          vs[i] = k;
          break;
        }
      }
    }
    for (unsigned k : vs) cout << s[k];
    cout << endl;
  }
  return 0;
}
