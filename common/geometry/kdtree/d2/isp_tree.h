#pragma once

#include "common/geometry/d3/point.h"
#include "common/geometry/kdtree/action/none.h"
#include "common/geometry/kdtree/d2/point_proxy.h"
#include "common/geometry/kdtree/idata/none.h"
#include "common/geometry/kdtree/info/none.h"
#include "common/geometry/kdtree/isp_tree.h"

namespace geometry {
namespace kdtree {
template <class TValue, class TLData = int64_t, class TIData = idata::None,
          class TInfo = info::None, class TAction = action::None>
using D2ISPTree =
    ISPTree<2u, geometry::d2::Point<TValue>, TLData, TIData, TInfo, TAction>;
}  // namespace kdtree
}  // namespace geometry
