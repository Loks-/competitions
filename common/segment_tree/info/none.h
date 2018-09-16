#pragma once

#include "proxy.h"

class TSTInfoNone
{
public:
	using TSelf = TSTInfoNone;

	static const bool is_none = true;
	static const bool use_data = false;
    static const bool has_segment = false;
	static const bool has_size = false;

	template<class TData>
	void UpdateData(const TData* data) {}

    void UpdateLR(const TSelf& l, const TSelf& r) {}
};

using STInfoNone = STInfoProxy<TSTInfoNone>;
