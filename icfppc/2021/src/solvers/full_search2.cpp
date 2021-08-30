#include "solvers/full_search2.h"

#include "evaluator.h"

#include "common/geometry/d2/distance/distance_l2.h"
#include "common/graph/flow/edge.h"
#include "common/graph/flow/graph.h"
#include "common/graph/flow/min_cost_flow.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace src_solvers;

void FullSearch2::InitSearch(const Problem& p) {
  TBase::InitSearch(p);
  ShuffleCandidatesOrder();
  best_score = (1 << 30);
}

bool FullSearch2::ForceStop() const {
  return (best_score == 0) || (t.GetSeconds() >= max_time_in_seconds) ||
         (cache.SegmentsMapSize() >= (1u << 23));
}

// Run MinCostFlow for approximate last step optimization.
void FullSearch2::LastStepOptimization() {
  if (ForceStop()) return;
  thread_local std::vector<unsigned> vd0;
  vd0.clear();
  for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
    if (used_vertices.HasKey(u)) continue;
    assert(remaining_order[u] == 0);
    vd0.push_back(u);
  }
  assert(vd0.size() > 0);
  unsigned L = problem.Hole().Size(), R = vd0.size(), extra = L + R,
           source = extra + 1, sink = source + 1;
  FlowGraph<graph::flow::Edge<int64_t, int>> g(sink + 1, source, sink);
  for (unsigned i = 0; i < L; ++i) {
    auto p = problem.Hole()[i];
    int64_t min_distance = (1ll << 60);
    for (unsigned u : used_vertices.List())
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
  Solution s{problem.Id(), solution};
  auto r = Evaluator::Apply(problem, s);
  assert(r.correct);
  if (r.correct && (r.score < best_score)) {
    best_score = r.score;
    best_solution = s;
  }
}

void FullSearch2::Run() {
  if (ForceStop()) return;
  unsigned next_u = used_vertices.SetSize();
  if (used_vertices.Size() == 0) {
    unsigned max_degree = 0;
    for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
      auto d = problem.Figure().Edges(u).size();
      if (max_degree < d) {
        max_degree = d;
        next_u = u;
      }
    }
  } else {
    unsigned min_size = cache.MaxIndex() + 1;
    for (unsigned u = 0; u < used_vertices.SetSize(); ++u) {
      if (used_vertices.HasKey(u)) continue;
      if (remaining_order[u] == 0) continue;
      auto csize = valid_candidates[u][valid_candidates_index[u]].size();
      if (min_size > csize) {
        min_size = csize;
        next_u = u;
      }
    }
    assert(min_size > 0);
  }
  if (next_u < used_vertices.SetSize()) {
    auto& vc = valid_candidates[next_u][valid_candidates_index[next_u]];
    for (auto& p : vc) {
      AddPoint(next_u, p);
      if (!force_stop) Run();
      RemoveLastPoint();
      if (ForceStop()) break;
    }
  } else {
    LastStepOptimization();
  }
}

Solution FullSearch2::Solve(const Problem& p) {
  std::cout << "Running FullSearch2::Solve for problem " << p.Id() << std::endl;
  t.Start();
  InitSearch(p);
  Run();
  std::cout << "\tBest score = " << int(best_score) << std::endl;
  return (best_score < (1 << 30)) ? best_solution : Solution();
}
