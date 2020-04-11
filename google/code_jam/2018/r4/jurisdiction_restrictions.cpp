#include "common/binary_search.h"
#include "common/data_structures/coordinate_compression.h"
#include "common/graph/flow/edge.h"
#include "common/graph/flow/graph.h"
#include "common/graph/flow/max_flow.h"
#include "common/stl/base.h"

int main_jurisdiction_restrictions() {
  struct PS {
    int64_t r, c, d;
  };

  unsigned T, S;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    int64_t R, C;
    cin >> R >> C >> S;
    vector<PS> stations;
    vector<int64_t> vr, vc;
    for (unsigned i = 0; i < S; ++i) {
      int64_t r, c, d;
      cin >> r >> c >> d;
      stations.push_back({r - 1, c - 1, d});
      vr.push_back(max<int64_t>(0, r - d - 1));
      vr.push_back(min<int64_t>(R, r + d));
      vc.push_back(max<int64_t>(0, c - d - 1));
      vc.push_back(min<int64_t>(C, c + d));
    }
    ds::CoordinateCompression<int64_t> ccr(vr), ccc(vc);
    unsigned sr = ccr.Size() - 1, sc = ccc.Size() - 1,
             sg = 2 + stations.size() + sr * sc;
    FlowGraph<FlowGraphEdge> g(sg, 0, sg - 1);
    int64_t max_flow = R * C;
    for (unsigned i = 0; i < stations.size(); ++i)
      g.AddEdge(0, i + 1, max_flow);
    for (unsigned r = 0; r < sr; ++r) {
      for (unsigned c = 0; c < sc; ++c) {
        unsigned index = 1 + stations.size() + r * sc + c;
        int64_t rb = ccr.GetOld(r), re = ccr.GetOld(r + 1), cb = ccc.GetOld(c),
                ce = ccc.GetOld(c + 1);
        int64_t s = (re - rb) * (ce - cb);
        for (unsigned i = 0; i < stations.size(); ++i) {
          auto ps = stations[i];
          if ((ps.r - ps.d <= rb) && (ps.r + ps.d + 1 >= re) &&
              (ps.c - ps.d <= cb) && (ps.c + ps.d + 1 >= ce))
            g.AddEdge(i + 1, index, max_flow);
          if ((ps.r >= rb) && (ps.r < re) && (ps.c >= cb) && (ps.c < ce)) --s;
        }
        if (s > 0) g.AddEdge(index, sg - 1, s);
      }
    }
    int64_t g_max_flow = MaxFlow(g);

    auto set_source_flow = [&](int64_t f) {
      g.ResetFlow();
      for (auto& e : g.Edges(0)) e.max_flow = f;
    };
    auto f1 = [&](int64_t f) {
      set_source_flow(f);
      auto gf = MaxFlow(g);
      return gf < f * stations.size();
    };
    auto f2 = [&](int64_t f) {
      set_source_flow(f);
      auto gf = MaxFlow(g);
      return gf == g_max_flow;
    };
    auto t1 = LowerBoundB<int64_t>(0, max_flow, f1);
    auto t2 = LowerBoundB<int64_t>(0, max_flow, f2);
    cout << "Case #" << it << ": " << t2 - t1 + 1 << endl;
  }
  return 0;
}
