#pragma once

#include <iostream>
#include <vector>

namespace {

template<class T>
std::vector<T> ReadVector(unsigned N)
{
	std::vector<T> v(N);
	for (unsigned i = 0; i < N; ++i)
		std::cin >> v[i];
	return v;
}

template<class T>
void WriteVector(const std::vector<T>& v)
{
	for (const T& t : v)
		std::cout << t << " ";
	std::cout << std::endl;
}

} // namespace
