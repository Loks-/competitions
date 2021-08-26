#pragma once

#include "cache.h"
#include "problem.h"
#include "solvers/base.h"

#include "common/data_structures/unsigned_set.h"
#include "common/geometry/d2/point.h"

namespace src_solvers {
// Support AddPoint / RemoveLastPoint and track valid candidates.
class BaseVCT : public Base {
 public:
  using TBase = Base;

 protected:
  Problem problem;
  Cache cache;
  ds::UnsignedSet used_vertices;
  std::vector<std::vector<std::vector<I2Point>>> valid_candidates;
  std::vector<unsigned> valid_candidates_index;
  std::vector<I2Point> solution;
  bool force_stop;

 protected:
  void InitSearch(const Problem& p);
  void ResetSearch();
  void AddPoint(unsigned index, const I2Point& p);
  void RemoveLastPoint();
  void AddPointFDC(unsigned index, const I2Point& p);
  void RemoveLastPointFDC();

 public:
  BaseVCT(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {}
};
}  // namespace src_solvers
