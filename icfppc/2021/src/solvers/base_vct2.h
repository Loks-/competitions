#pragma once

#include "problem.h"
#include "solvers/base_vct.h"

#include <vector>

namespace src_solvers {
// Track points remaning order.
class BaseVCT2 : public BaseVCT {
 public:
  using TBase = BaseVCT;

 protected:
  std::vector<int> remaining_order;

 protected:
  void InitSearch(const Problem& p);
  void ResetSearch();
  void AdjustNeighborsOrder(unsigned index, bool add_point);
  void ZeroOrderOptimization();
  void AddPoint(unsigned index, const I2Point& p);
  void RemoveLastPoint();
  void AddPointFDC(unsigned index, const I2Point& p);
  void RemoveLastPointFDC();

 public:
  BaseVCT2(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {}
};
}  // namespace src_solvers
