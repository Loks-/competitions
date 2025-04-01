#pragma once

#include "common/optimization/model/model.h"
#include "common/optimization/model/trainer/datapoint.h"

namespace opt {
namespace model {
namespace trainer {
class Trainer {
 public:
  template <class TDataPoint>
  void AddPoint(const TDataPoint&) {}

  void Train() {}
  Model GetModel() { return {}; }
};
}  // namespace trainer
}  // namespace model
}  // namespace opt
