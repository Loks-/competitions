#pragma once

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/vector.h"

#include <vector>

inline const std::vector<I2Vector>& I2NeighborsD2() {
  static std::vector<I2Vector> v{{1, 0}, {0, 1}};
  return v;
}

inline const std::vector<I2Vector>& I2NeighborsD3() {
  static std::vector<I2Vector> v{{1, 0}, {1, 1}, {0, 1}};
  return v;
}

inline const std::vector<I2Vector>& I2NeighborsD4() {
  static std::vector<I2Vector> v{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  return v;
}

inline const std::vector<I2Vector>& I2NeighborsD8() {
  static std::vector<I2Vector> v{{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                                 {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
  return v;
}

inline std::vector<I2Point> I2NeighborsD2(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD2()) v.push_back(p + d);
  return v;
}

inline std::vector<I2Point> I2NeighborsD3(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD3()) v.push_back(p + d);
  return v;
}

inline std::vector<I2Point> I2NeighborsD4(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD4()) v.push_back(p + d);
  return v;
}

inline std::vector<I2Point> I2NeighborsD8(const I2Point& p) {
  std::vector<I2Point> v;
  for (auto d : I2NeighborsD8()) v.push_back(p + d);
  return v;
}
