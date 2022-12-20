#pragma once

#include "common/geometry/ds/point.h"
#include "common/geometry/kdtree/action/none.h"
#include "common/geometry/kdtree/base/under.h"
#include "common/geometry/kdtree/base_tree.h"
#include "common/geometry/kdtree/idata/none.h"
#include "common/geometry/kdtree/info/none.h"

namespace geometry {
namespace kdtree {
template <unsigned dim, class TValue, class TLData = int64_t,
          class TIData = idata::None, class TInfo = info::None,
          class TAction = action::None>
using DSBaseTree = BaseTree<dim, geometry::ds::Point<dim, TValue>, TLData,
                            TIData, TInfo, TAction>;
}  // namespace kdtree
}  // namespace geometry
