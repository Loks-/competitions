#include "cache.h"

#include "problem.h"

#include "common/geometry/d2/point.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/box.h"
#include "common/geometry/d2/utils/inside_point_polygon.h"
#include "common/geometry/d2/utils/inside_segment_polygon.h"
#include "common/graph/graph_ei/distance_all_pairs_positive_cost.h"
#include "common/numeric/utils/usqrt.h"

#include <algorithm>

void Cache::Init(const Problem& problem) {
  hole = problem.Hole();
  box = Box(hole.v);
  valid_points_map.clear();
  valid_points_map.resize(box.p2.x - box.p1.x + 1);
  // Valid points
  for (unsigned i = 0; i < valid_points_map.size(); ++i) {
    valid_points_map[i].resize(box.p2.y - box.p1.y + 1, 0);
    for (unsigned j = 0; j < valid_points_map[i].size(); ++j) {
      I2Point p0(box.p1.x + i, box.p1.y + j);
      if (geometry::d2::Inside(p0, hole)) {
        valid_points_map[i][j] = 1;
        valid_points.push_back(p0);
      }
    }
  }
  // Init min/max distance between vertexes for figure.
  const auto& f = problem.Figure();
  UndirectedGraphEI<int64_t> gf(f.Size());
  for (unsigned u = 0; u < gf.Size(); ++u) {
    for (auto e : f.EdgesEI(u)) {
      gf.AddEdge(u, e.to, USqrt(e.info.second - 1) + 1);
    }
  }
  auto vvd = DistanceAllPairsPositiveCost(gf, static_cast<int64_t>(1ll << 30));
  figure_min_distance.clear();
  figure_min_distance.resize(gf.Size());
  figure_max_distance.clear();
  figure_max_distance.resize(gf.Size());
  for (unsigned i = 0; i < gf.Size(); ++i) {
    figure_min_distance[i].resize(gf.Size(), 0);
    figure_max_distance[i].resize(gf.Size());
    for (unsigned j = 0; j < gf.Size(); ++j)
      figure_max_distance[i][j] = vvd[i][j] * vvd[i][j];
  }
  for (unsigned u = 0; u < gf.Size(); ++u) {
    for (auto e : f.EdgesEI(u)) {
      figure_min_distance[u][e.to] = e.info.first;
      figure_max_distance[u][e.to] =
          std::min(figure_max_distance[u][e.to], e.info.second);
    }
  }
}

bool Cache::Test(const I2Point& p) const {
  if (!box.Inside(p)) return false;
  return valid_points_map[p.x - box.p1.x][p.y - box.p1.y];
}

bool Cache::Test(const I2ClosedSegment& s) {
  return Test(s.p1) && Test(s.p2) && TestI(s);
}

bool Cache::TestI(const I2ClosedSegment& s) {
  auto it = valid_segments_map.find(s);
  if (it != valid_segments_map.end()) return it->second;
  bool b = geometry::d2::Inside(s, hole);
  valid_segments_map.insert({s, b});
  return b;
}
