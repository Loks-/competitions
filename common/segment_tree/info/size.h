#pragma once

#include "proxy.h"

template<class TInfo>
class TSTInfoSize : public TInfo
{
public:
    using TBase = TInfo;
	using TSelf = STInfoSize<TBase>;

	static const bool is_none = false;
	static const bool has_size = true;

	unsigned size = 0;

    unsigned GetSize() const { return size; }

	template<class TData>
	void UpdateData(const TData* data) { TBase::UpdateData(data); size = 1; }

    void UpdateLR(const TSelf& l, const TSelf& r) { TBAse::UpdateLR(l, r); size = l.size + r.size; }
};

template<class TInfo>
using STInfoSize = STInfoProxy<TSTInfoSize<TInfo>>;
