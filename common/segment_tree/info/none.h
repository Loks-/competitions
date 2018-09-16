#pragma once

#include "proxy.h"

class TSTInfoNone
{
public:
	using TCoordinate = unsigned;
	using TSelf = TSTInfoNone;

	static const bool is_none = true;
	static const bool use_data = false;
    static const bool has_coordinate = false;

	void SetCoordinate(const TCoordinate& x) {}

	template<class TData>
	void UpdateData(const TData* data) {}

    void UpdateLR(const TSelf& l, const TSelf& r) {}
};

using STInfoNone = STInfoProxy<TSTInfoNone>;
