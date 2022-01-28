#pragma once

#include "common/base.h"
#include "common/geometry/affine_transformation.h"

using D3AffineTransformation = geometry::AffineTransformation<double, 3>;
using I3AffineTransformation = geometry::AffineTransformation<int64_t, 3>;
