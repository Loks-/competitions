#pragma once

#include "../../base.h"
#include "proxy.h"
#include <algorithm>

template<class TInfo, class TTCoordinate = int64_t>
class TSTInfoSegment : public TInfo
{
public:
    using TCoordinate = TTCoordinate;
    using TBase = TInfo;
    using TSelf = TSTInfoSegment<TBase, TCoordinate>;

	static const bool is_none = false;
	static const bool has_segment = true;

	TCoordinate xleft, xright;

    void SetCoordinates(const TCoordinate& x) { xleft = xright = x; }
    void SetCoordinates(const TCoordinate& xl, const TCoordinate& xr) { xleft = xl; xright = xr; }

    void UpdateLR(const TSelf& l, const TSelf& r)
    {
        TBase::UpdateLR(l, r);
        xleft = std::min(l.xleft, r.xleft);
        xright = std::max(l.xright, r.xright);
    }
};

template<class TInfo, class TCoordinate = int64_t>
using STInfoSegment = STInfoProxy<TSTInfoSegment<TInfo, TCoordinate>>;
