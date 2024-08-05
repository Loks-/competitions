#pragma once

#include "common/numeric/complex.h"

#include <cmath>

using DComplex = numeric::Complex<double>;

constexpr double Abs(const DComplex& x) { return sqrt(x.AbsSquared()); }
