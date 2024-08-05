#pragma once

#include "common/geometry/d3/base.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

#include <vector>

constexpr std::vector<I3Vector> I3NeighborsD3() {
  return {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
}

constexpr std::vector<I3Vector> I3NeighborsD6() {
  return {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
}

constexpr std::vector<I3Point> I3NeighborsD3(const I3Point& p) {
  std::vector<I3Point> v;
  for (auto d : I3NeighborsD3()) v.push_back(p + d);
  return v;
}

constexpr std::vector<I3Point> I3NeighborsD6(const I3Point& p) {
  std::vector<I3Point> v;
  for (auto d : I3NeighborsD6()) v.push_back(p + d);
  return v;
}
