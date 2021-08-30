#include "solvers/base_vct2.h"

#include "common/base.h"
#include "common/geometry/d2/distance/distance_l2.h"
#include "common/geometry/d2/segment.h"

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
    if (!used_vertices.HasKey(u)) remaining_order[u] += shift;
  }
}

void BaseVCT2::AddPoint(unsigned index, const I2Point& p) {
  TBase::AddPoint(index, p);
  AdjustNeighborsOrder(index, true);
}

void BaseVCT2::RemoveLastPoint() {
  unsigned index = used_vertices.Last();
  AdjustNeighborsOrder(index, false);
  TBase::RemoveLastPoint();
}

void BaseVCT2::AddPointFDC(unsigned index, const I2Point& p) {
  TBase::AddPointFDC(index, p);
  AdjustNeighborsOrder(index, true);
}

void BaseVCT2::RemoveLastPointFDC() {
  unsigned index = used_vertices.Last();
  AdjustNeighborsOrder(index, false);
  TBase::RemoveLastPointFDC();
}
