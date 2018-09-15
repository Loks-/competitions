#pragma once

#include "none.h"
#include "proxy.h"

class TSTInfoSize : public TSTInfoNone
{
public:
	static const bool is_none = false;
	static const bool has_size = true;

	unsigned size = 0;

    unsigned GetSize() const { return size; }

	template<class TData>
	void UpdateData(const TData* data) { size = 1; }

    void UpdateLR(const TSelf& l, const TSelf& r) { size = l.size + r.size; }
};

using STInfoSize = STInfoProxy<TSTInfoSize>;
