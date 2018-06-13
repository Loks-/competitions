#pragma once

#include "../base.h"

class VectorUtils
{
public:
	template<typename T>
	static vector<T> ReadVector(unsigned N)
	{
		vector<T> v(N);
		for (unsigned i = 0; i < N; ++i)
			cin >> v[i];
		return v;
	}
};
