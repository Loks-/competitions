#include "common/geometry/d2/distance/distance_l1.h"
#include "common/geometry/d2/point.h"
#include "common/graph/flow/edge.h"
#include "common/graph/flow/graph.h"
#include "common/graph/flow/min_cost_flow.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <string>

int main_retiling__mcf() {
  unsigned T, R, C, F, S;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> C >> F >> S;
    auto vs = nvector::Read<string>(R);
    auto ve = nvector::Read<string>(R);
    std::vector<int> vr;
    for (unsigned ic = 0; ic < 2; ++ic) {
      char c = (ic == 0) ? 'M' : 'G';
      vector<I2Point> vps, vpe;
      for (unsigned i = 0; i < R; ++i) {
        for (unsigned j = 0; j < C; ++j) {
          if (vs[i][j] == c) vps.push_back({i, j});
          if (ve[i][j] == c) vpe.push_back({i, j});
        }
      }
      if (vps.size() > vpe.size()) vps.swap(vpe);
      unsigned n = vps.size(), m = vpe.size(), source = n + m, sink = n + m + 1;
      FlowGraph<graph::flow::Edge<int, int>> g(n + m + 2, source, sink);
      for (unsigned i = 0; i < n; ++i) {
        g.AddDataEdge(0, source, i, 1);
        for (unsigned j = 0; j < m; ++j)
          g.AddDataEdge(min<int>(2 * F, S * DistanceL1(vps[i], vpe[j])), i,
                        j + n, 1);
      }
      for (unsigned j = 0; j < m; ++j) g.AddDataEdge(0, j + n, sink, 1);
      auto r = MinCostFlow(g, n) + F * (m - n);
      assert(g.Flow() == int(n));
      vr.push_back(r);
    }
    cout << "Case #" << it << ": " << min(vr[0], vr[1]) << endl;
  }
  return 0;
}
