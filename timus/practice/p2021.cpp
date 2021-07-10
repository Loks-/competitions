#include "common/base.h"
#include "common/geometry/d2/polygon.h"
#include "common/graph/graph_ei.h"
#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/utils/inside_segment_polygon.h"
#include "common/geometry/d2/utils/inside_point_polygon.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/vector.h"
#include "common/numeric/utils/abs.h"
#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/utils/box.h"
#include "common/geometry/d2/stl_hash/segment.h"
#include "common/data_structures/unsigned_set.h"

#include <algorithm>
#include <vector>
#include <unordered_set>

class Solution {
 public:
  std::vector<I2Point> points;
};

class Task {
public:
  I2Polygon hole;
  UndirectedGraphEI<int64_t> g;
  unsigned eps;

  bool CheckDistance(int64_t required, int64_t current) const {
    return Abs(current - required) * 1000000ll <= required * eps;
  }

  bool IsValid(const Solution& s) const {
    if (s.points.size() != g.Size()) return false;
    for (unsigned u = 0; u < g.Size(); ++u) {
      auto p0 = s.points[u];
      for (auto e : g.EdgesEI(u)) {
        if (e.to < u) continue;
        auto p1 = s.points[e.to];
        auto d2 = SquaredDistanceL2(p0, p1);
        if (!CheckDistance(e.info, d2)) return false;
        if (!geometry::d2::Inside(I2ClosedSegment(p0, p1), hole)) return false;
      }
    }
    return true;
  }

  int64_t Score(const Solution& s) const {
    int64_t score = 0;
    for (unsigned i = 0; i < hole.Size(); ++i) {
      auto p0 = hole[i];
      int64_t min_d = SquaredDistanceL2(p0, s.points[0]);
      for (unsigned j = 1; j < s.points.size(); ++j) {
        int64_t d = SquaredDistanceL2(p0, s.points[j]);
        min_d = std::min(min_d, d);
      }
      score += min_d;
    }
    return score;
  }
};

class TaskCache {
 protected:
  I2ARectangle box;
  std::vector<std::vector<unsigned>> valid_points_map;
  std::vector<I2Point> valid_points;
  std::vector<std::vector<std::vector<I2Point>>> valid_segments_map;
  std::unordered_set<I2ClosedSegment> valid_segments_set;

 public:
  const std::vector<I2Point>& GetValidPoints() const {
    return valid_points;
  }

  void Init(const Task& task) {
    box = Box(task.hole.v);
    valid_points_map.clear();
    valid_points_map.resize(box.p2.x - box.p1.x + 1);
    valid_segments_map.clear();
    valid_segments_map.resize(valid_points_map.size());
    for (unsigned i = 0; i < valid_points_map.size(); ++i) {
      valid_points_map[i].resize(box.p2.y - box.p1.y + 1, 0);
      valid_segments_map[i].resize(valid_points_map[i].size());
      for (unsigned j = 0; j < valid_points_map[i].size(); ++j) {
        I2Point p0(box.p1.x + i, box.p1.y + j);
        if (geometry::d2::Inside(p0, task.hole)) {
          valid_points_map[i][j] = 1;
          valid_points.push_back(p0);
        }
      }
    }
    for (auto p1 : valid_points) {
      for (auto p2: valid_points) {
        I2ClosedSegment s(p1, p2);
        if (geometry::d2::Inside(s, task.hole)) {
          valid_segments_map[p1.x - box.p1.x][p1.y - box.p1.y].push_back(p2);
          valid_segments_set.insert(s);
        }
      }
    }
  }

  bool CheckPoint(const I2Point& p) const {
    if (!box.Inside(p)) return false;
    return valid_points_map[p.x - box.p1.x][p.y - box.p1.y];
  }

  bool CheckSegment(const I2ClosedSegment& s) const {
    return valid_segments_set.find(s) != valid_segments_set.end();
  }
};

class GreedySolver {
 protected:
  Task task;
  TaskCache cache;
  ds::UnsignedSet used_vertices;
  std::vector<std::vector<std::vector<I2Point>>> valid_candidates;
  std::vector<I2Point> solution;

 public:
  GreedySolver(const Task& _task) {
    task = _task;
    cache.Init(task);
    ResetSearch();
  }

  GreedySolver(const Task& _task, const TaskCache& _cache) {
    task = _task;
    cache = _cache;
    ResetSearch();
  }

  void ResetSearch() {
    unsigned size = task.g.Size();
    used_vertices.Resize(size);
    used_vertices.Clear();
    valid_candidates.clear();
    valid_candidates.resize(size);
    for (unsigned i = 0; i < size; ++i) {
      valid_candidates[i].push_back(cache.GetValidPoints());
    }
    solution.resize(size);
  }

  void AddPoint(unsigned index, const I2Point& p) {
    assert(!used_vertices.HasKey(index));
    used_vertices.Insert(index);
    solution[index] = p;
    for (auto e : task.g.EdgesEI(index)) {
      unsigned u = e.to;
      if (used_vertices.HasKey(u)) {
        // Verify only
        auto p1 = solution[u];
        auto d = SquaredDistanceL2(p, p1);
        if (!task.CheckDistance(e.info, d) || !cache.CheckSegment(I2ClosedSegment(p, p1))) {
          std::cout << "SUS!" << std::endl;
          assert(false);    
        }
      } else {
        // Filter points
        std::vector<I2Point> vnext;
        auto& vcurrent = valid_candidates[u].back();
        vnext.reserve(vcurrent.size());
        for (auto p1 : vnext) {
          auto d = SquaredDistanceL2(p, p1);
          if (task.CheckDistance(e.info, d) && cache.CheckSegment(I2ClosedSegment(p, p1)))
            vnext.push_back(p1);
        }
        valid_candidates[u].emplace_back(vnext);
      }
    }
  }
  
  void RemoveLastPoint() {
    unsigned index = used_vertices.Last();
    for (auto e : task.g.EdgesEI(index)) {
      unsigned u = e.to;
      if (!used_vertices.HasKey(u)) {
        valid_candidates[u].pop_back();
      }
    }
    used_vertices.RemoveLast();
  }

  bool Search() {
    if (used_vertices.Size() == task.g.Size()) {
      return false;
    }
    unsigned min_size = cache.GetValidPoints().size() + 1;
    unsigned min_index = used_vertices.SetSize();
    for (unsigned i = 0; i < used_vertices.SetSize(); ++i) {
      if (used_vertices.HasKey(i)) continue;
      if (valid_candidates[i].back().size() < min_size) {
        min_size = valid_candidates[i].back().size();
        min_index = i;
      }
    }
    if (min_size == 0) return {};
    auto v = valid_candidates[min_index].back(); // for safety
    for (auto& p : v) {
      AddPoint(min_index, p);
      if (Search()) return true;
      RemoveLastPoint();
    }
    return false;
  }
};

int main_p2021() {
  return 0;
}
