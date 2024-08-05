#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

#include <vector>

constexpr std::vector<I2Vector> I2NeighborsD2() { return {{1, 0}, {0, 1}}; }

constexpr std::vector<I2Vector> I2NeighborsD3() {
  return {{1, 0}, {1, 1}, {0, 1}};
}

constexpr std::vector<I2Vector> I2NeighborsD4() {
  return {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
}

constexpr std::vector<I2Vector> I2NeighborsD8() {
  return {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
}

constexpr std::vector<I2Point> I2NeighborsD2(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD2()) v.push_back(p + d);
  return v;
}

constexpr std::vector<I2Point> I2NeighborsD3(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD3()) v.push_back(p + d);
  return v;
}

constexpr std::vector<I2Point> I2NeighborsD4(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD4()) v.push_back(p + d);
  return v;
}

constexpr std::vector<I2Point> I2NeighborsD8(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD8()) v.push_back(p + d);
  return v;
}
