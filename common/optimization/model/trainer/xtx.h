#pragma once

#include "common/base.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/real/lup_decomposition.h"
#include "common/linear_algebra/vector.h"
#include "common/optimization/model/linear.h"
#include "common/optimization/model/trainer/datapoint.h"
#include "common/optimization/model/trainer/trainer.h"
#include "common/template.h"

namespace opt {
namespace model {
namespace trainer {
class XTX : public Trainer {
 public:
  using TDataPoint = DataPoint<DVector, double>;

 protected:
  double ridge_add;
  double ridge_mult;
  DMatrix xtx, temp;
  DVector xty;
  double y2;
  la::real::LUPDecomposition<DMatrix> d;
  Linear m;

 public:
  XTX(double _ridge_add = 1e-6, double _ridge_mult = 1e-6)
      : xtx(0), temp(0), xty(0) {
    SetRidge(_ridge_add, _ridge_mult);
    Clear();
  }

  void Clear() {
    xtx.Clear();
    xty.Clear();
    y2 = 0.;
  }

  void SetRidge(double add, double mult) {
    ridge_add = add;
    ridge_mult = mult;
  }

  void Init(unsigned size) {
    xtx = DMatrix(size, size, 0.);
    xty = DVector(size, 0.);
    m.b = DVector(size, 0.);
  }

  void Decay(double decay) {
    xtx *= decay;
    xty *= decay;
    y2 *= decay;
  }

  void AddPoint(const TDataPoint& p) {
    // TODO: Support weight
    if (xtx.Size() == 0) Init(p.x.Size());
    xtx.AddXXT(p.x);
    xty += p.x * p.y;
    y2 += p.y * p.y;
  }

  void Train() {
    temp = xtx;
    for (unsigned i = 0; i < temp.Rows(); ++i) {
      temp(i, i) = (1.0 + ridge_mult) * temp(i, i) + ridge_add;
    }
    bool b1 = d.Build(temp);
    bool b2 = d.Solve(xty, m.b);
    assert(b1 && b2);
    FakeUse(b1 == b2);
  }

  Linear GetModel() { return m; }

  double Variance() const { return y2; }

  double Error() const {
    auto t = xtx * m.b;
    return m.b.DotProduct(t - xty * 2.0) + y2;
  }

  double R2() const { return (y2 > 0) ? 1.0 - Error() / Variance() : 0.; }
};
}  // namespace trainer
}  // namespace model
}  // namespace opt
