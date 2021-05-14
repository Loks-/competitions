#pragma once

namespace opt {
namespace model {
namespace trainer {
template <class TDataX, class TTarget, class TWeight = double>
class DataPoint {
 public:
  TDataX x;
  TTarget y;
  TWeight w;

  DataPoint(const TDataX& _x, const TTarget& _y) : x(_x), y(_y), w(1) {}
  DataPoint(const TDataX& _x, const TTarget& _y, const TWeight& _w)
      : x(_x), y(_y), w(_w) {}
};
}  // namespace trainer
}  // namespace model
}  // namespace opt
