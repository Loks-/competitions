#pragma once

#include "../base.h"

namespace {

template<typename T>
vector<T> ReadVector(unsigned N)
{
	vector<T> v(N);
	for (unsigned i = 0; i < N; ++i)
		cin >> v[i];
	return v;
}

} // namespace
