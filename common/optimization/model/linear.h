#pragma once

#include "common/linear_algebra/vector.h"
#include "common/optimization/model/model.h"

namespace opt {
namespace model {
class Linear : public Model {
 public:
  DVector b;

 public:
  Linear() : b(0) {}

  double Apply(const DVector& x) const { return b.DotProduct(x); }
};
}  // namespace model
}  // namespace opt
