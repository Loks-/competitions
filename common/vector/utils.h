#pragma once

#include "../base.h"

namespace {

template<class T>
vector<T> ReadVector(unsigned N)
{
	vector<T> v(N);
	for (unsigned i = 0; i < N; ++i)
		cin >> v[i];
	return v;
}

template<class T>
void WriteVector(const vector<T>& v)
{
	for (const T& t : v)
		cout << t << " ";
	cout << endl;
}

} // namespace
