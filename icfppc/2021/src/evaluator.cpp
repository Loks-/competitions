#include "evaluator.h"

#include "problem.h"
#include "solution.h"

#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/location/point_polygon.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/utils/inside_point_polygon.h"
#include "common/geometry/d2/utils/inside_segment_polygon.h"

#include <algorithm>
#include <string>

// TODO:
//   Code for GLOBALIST and BREAK_A_LEG bonuses.
Evaluator::Result Evaluator::Apply(const Problem& p, const Solution& s) {
  auto& hole = p.Hole();
  auto g = p.Figure();
  auto bonus_type = s.GetBonusUsed().Type();
  auto& points = s.GetPoints();
  unsigned outside_vertexes = 0,
           max_outside_vertexes = (bonus_type == Bonus::WALLHACK) ? 1 : 0;
  unsigned out_of_range_edges = 0,
           max_out_of_range_edges = (bonus_type == Bonus::GLOBALIST)
                                        ? g.EdgesSize()
                                    : (bonus_type == Bonus::SUPERFLEX) ? 1
                                                                       : 0;
  if (g.Size() != points.size()) return {false, -11};
  for (auto& p : points) {
    if (!geometry::d2::Inside(p, hole)) {
      std::cerr << "Point " << p << " outside of polygon." << std::endl;
      auto l = geometry::d2::location::Locate(p, hole);
      std::cerr << unsigned(l.type) << std::endl;
      if (++outside_vertexes > max_outside_vertexes) return {false, -12};
    }
  }
  for (unsigned u = 0; u < g.Size(); ++u) {
    auto p1 = points[u];
    for (auto e : g.EdgesEI(u)) {
      auto p2 = points[e.to];
      if (geometry::d2::Inside(p1, hole) && geometry::d2::Inside(p2, hole)) {
        if (!geometry::d2::Inside(I2ClosedSegment(p1, p2), hole))
          return {false, -13};
      }
      auto d2 = SquaredDistanceL2(p1, p2);
      if ((d2 < e.info.first) || (d2 > e.info.second)) {
        // std::cerr << "SL2 =\t" << e.info.first << "\t" << d2 << "\t"
        //           << e.info.second << std::endl;
        if (++out_of_range_edges > max_out_of_range_edges) return {false, -14};
      }
    }
  }
  int64_t score = 0;
  for (auto& p1 : hole.v) {
    int64_t min_distance = (1ll << 60);
    for (auto& p2 : points)
      min_distance = std::min(min_distance, SquaredDistanceL2(p1, p2));
    score += min_distance;
  }
  return {true, score};
}
