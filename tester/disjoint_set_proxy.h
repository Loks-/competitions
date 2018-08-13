#pragma once

#include "common/disjoint_set.h"

enum EPathCompression
{
	pc_none = 0,
	pc_compression_recursive,
	pc_compression_two_runs,
	pc_compression_stack,
};

template<EPathCompression pc>
class DisjointSetProxy : public DisjointSet
{
public:
	using TBase = DisjointSet;

	DisjointSetProxy(unsigned size) : TBase(size) {}

	void Union(unsigned i1, unsigned i2)
	{
		TBase::UnionI(FindP(i1), FindP(i2));
	}

	unsigned FindP(unsigned x);
};

template<>
unsigned DisjointSetProxy<pc_none>::FindP(unsigned x)
{
	for (; x != TBase::p[x]; ) x = TBase::p[x];
	return x;
}

template<>
unsigned DisjointSetProxy<pc_compression_recursive>::FindP(unsigned x)
{
	if (x == TBase::p[x])
		return x;
	TBase::p[x] = FindP(TBase::p[x]);
	return TBase::p[x];
}

template<>
unsigned DisjointSetProxy<pc_compression_stack>::FindP(unsigned x)
{
	unsigned px = TBase::p[x];
	if (px == x)
		return px;
	unsigned ppx = TBase::p[px];
	if (ppx == px)
		return px;
	do
	{
		TBase::ts.push(x);
		x = px;
		px = ppx;
		ppx = TBase::p[px];
	} while (px != ppx);
	while (!TBase::ts.empty())
	{
		x = TBase::ts.top();
		TBase::p[x] = ppx;
		TBase::ts.pop();
	}
	return ppx;
}
