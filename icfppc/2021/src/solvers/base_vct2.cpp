#include "solvers/base_vct2.h"

#include "common/base.h"
#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/segment.h"
#include "common/graph/flow/edge.h"
#include "common/graph/flow/graph.h"
#include "common/graph/flow/min_cost_flow.h"

#include <algorithm>
#include <iostream>
#include <random>

using namespace src_solvers;

void BaseVCT2::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  auto& g = problem.Figure();
  remaining_order.resize(g.Size());
  for (unsigned u = 0; u < g.Size(); ++u)
    remaining_order[u] = int(g.Edges(u).size());
}

void BaseVCT2::ResetSearch() {
  TBase::ResetSearch();
  auto& g = problem.Figure();
  for (unsigned u = 0; u < g.Size(); ++u)
    remaining_order[u] = int(g.Edges(u).size());
}

void BaseVCT2::AdjustNeighborsOrder(unsigned index, bool add_point) {
  int shift = add_point ? -1 : 1;
  for (auto u : problem.Figure().Edges(index)) {
    if (unused_vertices.HasKey(u)) remaining_order[u] += shift;
  }
}

// Run MinCostFlow for approximate last step optimization.
void BaseVCT2::ZeroOrderOptimization() {
  thread_local std::vector<unsigned> vd0;
  vd0.clear();
  for (unsigned u : unused_vertices.List()) {
    assert(remaining_order[u] == 0);
    vd0.push_back(u);
  }
  if (vd0.size() == 0) return;
  unsigned L = problem.Hole().Size(), R = vd0.size(), extra = L + R,
           source = extra + 1, sink = source + 1;
  FlowGraph<graph::flow::Edge<int64_t, int>> g(sink + 1, source, sink);
  for (unsigned i = 0; i < L; ++i) {
    auto p = problem.Hole()[i];
    int64_t min_distance = (1ll << 60);
    for (unsigned u : used_vertices)
      min_distance = std::min(min_distance, SquaredDistanceL2(p, solution[u]));
    g.AddDataEdge(0, source, i, 1);
    g.AddDataEdge(min_distance, i, extra, 1);
    for (unsigned j = 0; j < R; ++j) {
      unsigned u = vd0[j];
      auto& vc = valid_candidates[u][valid_candidates_index[u]];
      min_distance = (1ll << 60);
      for (auto pu : vc)
        min_distance = std::min(min_distance, SquaredDistanceL2(p, pu));
      g.AddDataEdge(min_distance, i, L + j, 1);
    }
  }
  for (unsigned j = 0; j < R; ++j) g.AddDataEdge(0, L + j, sink, 1);
  g.AddDataEdge(0, extra, sink, L);
  MinCostFlow(g, L);
  assert(g.Flow() == int(L));
  for (unsigned j = 0; j < R; ++j) {
    unsigned u = vd0[j];
    auto& vc = valid_candidates[u][valid_candidates_index[u]];
    bool used = false;
    for (auto& e : g.Edges(L + j)) {
      if ((e.flow == 0) || (e.to == sink)) continue;
      assert((e.flow == -1) && (e.to < L));
      auto p = problem.Hole()[e.to];
      I2Point pbest;
      int64_t min_distance = (1ll << 60);
      for (auto pu : vc) {
        auto d = SquaredDistanceL2(p, pu);
        if (d < min_distance) {
          min_distance = d;
          pbest = pu;
        }
      }
      solution[u] = pbest;
      used = true;
      break;
    }
    if (!used) solution[u] = vc[0];
  }
}

void BaseVCT2::AddPoint(unsigned index, const I2Point& p) {
  TBase::AddPoint(index, p);
  AdjustNeighborsOrder(index, true);
}

void BaseVCT2::RemoveLastPoint() {
  unsigned index = used_vertices.back();
  AdjustNeighborsOrder(index, false);
  TBase::RemoveLastPoint();
}

void BaseVCT2::AddPointFDC(unsigned index, const I2Point& p) {
  TBase::AddPointFDC(index, p);
  AdjustNeighborsOrder(index, true);
}

void BaseVCT2::RemoveLastPointFDC() {
  unsigned index = used_vertices.back();
  AdjustNeighborsOrder(index, false);
  TBase::RemoveLastPointFDC();
}
