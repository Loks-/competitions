#pragma once

#include <cmath>

namespace opt {
namespace functions {
double Sigmoid(double x) {
  return (x > 0) ? 1.0 - Sigmoid(-x) : (x < -100) ? 0. : 1.0 / (1.0 + exp(-x));
}

double LogSigmoid(double x) {
  return (x >= 0) ? -log1p(exp(-x)) : x + LogSigmoid(-x);
}
}  // namespace functions
}  // namespace opt
