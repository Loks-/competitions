#pragma once

namespace opt {
namespace model {
class Model {
 public:
  template <class TVector>
  double Apply(const TVector& x) const {
    return 0.;
  }
};
}  // namespace model
}  // namespace opt
