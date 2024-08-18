#include "common/geometry/d2/base.h"
#include "common/geometry/d2/hash/point.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/vector.h"
#include "common/graph/graph_ei.h"
#include "common/graph/plane/dual_ei.h"
#include "common/graph/plane/sort_edges_by_angle.h"
#include "common/hash.h"
#include "common/heap.h"
#include "common/stl/base.h"
#include "common/vector/write.h"

#include <unordered_map>

int main_fence_construction() {
  struct EdgeInfo {
    I2Vector v;
    unsigned index;
  };

  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned F, K, points = 0;
    cin >> F >> K;
    unordered_map<I2Point, unsigned, DHash<I2Point>> pmap;

    auto point_id = [&](const I2Point& p) {
      auto it = pmap.find(p);
      if (it != pmap.end()) return it->second;
      return pmap[p] = points++;
    };

    vector<pair<I2Point, I2Point>> fences;
    for (unsigned i = 0; i < F; ++i) {
      I2Point p1, p2;
      cin >> p1 >> p2;
      point_id(p1);
      point_id(p2);
      fences.push_back({p1, p2});
    }
    UndirectedGraphEI<EdgeInfo> g(points);
    for (unsigned i = 0; i < F; ++i) {
      I2Point p1 = fences[i].first, p2 = fences[i].second;
      unsigned u1 = point_id(p1), u2 = point_id(p2);
      g.AddBaseEdge(u1, u2, {p2 - p1, i + 1});
      g.AddBaseEdge(u2, u1, {p1 - p2, i + 1});
    }
    SortEdgesByAngle(g);
    auto gd = Dual(g);
    vector<pair<unsigned, unsigned>> edges(F + 1);
    for (unsigned i = 0; i < gd.Size(); ++i) {
      for (const auto& e : gd.EdgesEI(i)) {
        edges[e.info.index] = {i, e.to};
      }
    }
    vector<unsigned> order;
    vector<unsigned> visited_edges(F + 2, 0), visited_faces(gd.Size(), 0);
    visited_edges[K] = 1;
    HeapMaxOnTop<unsigned> h;
    for (h.Add(K); !h.Empty();) {
      unsigned eid = h.Extract();
      if ((eid < K) && !visited_edges[eid + 1]) break;
      order.push_back(eid);
      for (unsigned u : {edges[eid].first, edges[eid].second}) {
        if (!visited_faces[u]) {
          visited_faces[u] = 1;
          for (const auto& e : gd.EdgesEI(u)) {
            if (!visited_edges[e.info.index]) {
              visited_edges[e.info.index] = 1;
              h.Add(e.info.index);
            }
          }
        }
      }
    }
    reverse(order.begin(), order.end());
    cout << "Case #" << it << ": ";
    nvector::Write(order);
  }
  return 0;
}
