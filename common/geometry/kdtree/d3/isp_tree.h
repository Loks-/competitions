#pragma once

#include "common/geometry/d3/point.h"
#include "common/geometry/kdtree/d3/get_set.h"
#include "common/geometry/kdtree/idata/none.h"
#include "common/geometry/kdtree/info/none.h"
#include "common/geometry/kdtree/isp_tree.h"

namespace geometry {
namespace kdtree {
template <unsigned _dim, class TValue, class TLData = int64_t,
          class TIData = idata::None, class TInfo = info::None,
          class TAction = TEmpty>
using D3ISPTree =
    ISPTree<3u, geometry::d3::Point<TValue>, TLData, TIData, TInfo, TAction>;
}  // namespace kdtree
}  // namespace geometry
