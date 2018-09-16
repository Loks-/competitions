#pragma once

#include "proxy.h"
#include <algorithm>

template<class TInfo, class TMaxValue>
class TSTInfoMax : public TInfo
{
public:
    using TValue = TMaxValue;
    using TBase = TInfo;
	using TSelf = TSTInfoMax<TBase, TValue>;

	static const bool is_none = false;
	static const bool use_data = true;

	TMaxValue segment_max;

	template<class TData>
	void UpdateData(const TData* data) { TBase::UpdateData(data); segment_max = *data; }

    void UpdateLR(const TSelf& l, const TSelf& r)
    {
        TBase::UpdateLR(l, r);
        segment_max = std::max(l.segment_max, r.segment_max);
    }
};

template<class TInfo, class TMaxValue>
using STInfoMax = STInfoProxy<TSTInfoMax<TInfo, TMaxValue>>;
