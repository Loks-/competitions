#include "solvers/base_vct.h"

#include "common/base.h"
#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/segment.h"

#include <algorithm>
#include <iostream>

using namespace src_solvers;

void BaseVCT::InitSearch(const Problem& p) {
  problem = p;
  cache.Init(problem);
  unsigned size = problem.Figure().Size();
  used_vertices.Resize(size);
  valid_candidates.clear();
  valid_candidates.resize(size);
  for (unsigned i = 0; i < size; ++i) {
    valid_candidates[i].resize(2 * size + 1);  // For safety
    valid_candidates[i][0] = cache.GetValidPoints();
  }
  valid_candidates_index.clear();
  valid_candidates_index.resize(size, 0);
  solution.clear();
  solution.resize(size);
  force_stop = false;
}

void BaseVCT::ResetSearch() {
  used_vertices.Clear();
  std::fill(valid_candidates_index.begin(), valid_candidates_index.end(), 0);
  force_stop = false;
}

void BaseVCT::AddPoint(unsigned index, const I2Point& p) {
  assert(!used_vertices.HasKey(index));
  used_vertices.Insert(index);
  solution[index] = p;
  for (auto e : problem.Figure().EdgesEI(index)) {
    unsigned u = e.to;
    if (used_vertices.HasKey(u)) {
      // Verify only
      auto p1 = solution[u];
      auto d = SquaredDistanceL2(p, p1);
      if ((d < e.info.first) || (d > e.info.second) ||
          !cache.Test(I2ClosedSegment(p, p1))) {
        std::cout << "SUS!" << std::endl;
        assert(false);
      }
    } else {
      // Filter points
      auto& vcurrent = valid_candidates[u][valid_candidates_index[u]];
      auto& vnext = valid_candidates[u][++valid_candidates_index[u]];
      vnext.clear();
      for (auto p1 : vcurrent) {
        auto d = SquaredDistanceL2(p, p1);
        if ((d >= e.info.first) && (d <= e.info.second) &&
            cache.TestI(I2ClosedSegment(p, p1))) {
          vnext.push_back(p1);
        }
      }
      if (vnext.empty()) force_stop = true;
    }
  }
}

void BaseVCT::RemoveLastPoint() {
  unsigned index = used_vertices.Last();
  for (auto e : problem.Figure().EdgesEI(index)) {
    unsigned u = e.to;
    if (!used_vertices.HasKey(u)) {
      --valid_candidates_index[u];
    }
  }
  used_vertices.RemoveLast();
  force_stop = false;
}
