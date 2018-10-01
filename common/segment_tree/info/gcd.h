#pragma once

#include "proxy.h"
#include "../../numeric/utils/gcd.h"

template<class TGCDValue, class TInfo>
class TSTInfoGCD : public TInfo
{
public:
    using TValue = TGCDValue;
    using TBase = TInfo;
	using TSelf = TSTInfoGCD<TValue, TBase>;

	static const bool is_none = false;
	static const bool use_data = true;

	TValue segment_gcd;

	template<class TData>
	void UpdateData(const TData& data) { TBase::UpdateData(data); segment_gcd = data; }

    void UpdateLR(const TSelf& l, const TSelf& r)
    {
        TBase::UpdateLR(l, r);
        segment_gcd = GCD<TValue>(l.segment_gcd, r.segment_gcd);
    }
};

template<class TGCDValue, class TInfo>
using STInfoGCD = STInfoProxy<TSTInfoGCD<TGCDValue, TInfo>>;
