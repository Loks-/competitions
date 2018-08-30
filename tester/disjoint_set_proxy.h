#pragma once

#include "common/disjoint_set.h"
#include <stack>

enum EPathCompression
{
	pc_none = 0,
	pc_default,
	pc_compression_recursive,
	pc_compression_two_runs,
	pc_compression_stack,
	pc_halving,
	pc_splitting
};

template<EPathCompression pc>
class DisjointSetProxy : public DisjointSet
{
protected:
    std::stack<unsigned> ts;

public:
	using TBase = DisjointSet;

	DisjointSetProxy(unsigned size) : TBase(size) {}
	void Union(unsigned i1, unsigned i2) { TBase::UnionI(FindP(i1), FindP(i2)); }
	unsigned FindP(unsigned x);
};

template<>
unsigned DisjointSetProxy<pc_none>::FindP(unsigned x)
{
	for (; x != TBase::p[x]; )
		x = TBase::p[x];
	return x;
}

template<>
unsigned DisjointSetProxy<pc_default>::FindP(unsigned x) { return TBase::Find(x); }

template<>
unsigned DisjointSetProxy<pc_compression_recursive>::FindP(unsigned x)
{
	unsigned p = TBase::p[x]; 
	if (x != p)
	{
		p = FindP(p);
		TBase::p[x] = p;
	}
	return p;
}

template<>
unsigned DisjointSetProxy<pc_compression_two_runs>::FindP(unsigned x)
{
	unsigned p = x, t;
	for (; p != TBase::p[p]; )
		p = TBase::p[p];
	for (; x != p; )
	{
		t = TBase::p[x];
		TBase::p[x] = p;
		x = t;
	}
	return p;
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
		ts.push(x);
		x = px;
		px = ppx;
		ppx = TBase::p[px];
	} while (px != ppx);
	while (!ts.empty())
	{
		x = ts.top();
		TBase::p[x] = ppx;
		ts.pop();
	}
	return ppx;
}

template<>
unsigned DisjointSetProxy<pc_halving>::FindP(unsigned x)
{
	for (unsigned p = TBase::p[x]; x != p; p = TBase::p[x])
	{
		p = TBase::p[p];
		TBase::p[x] = p;
		x = p;
	}
	return x;
}

template<>
unsigned DisjointSetProxy<pc_splitting>::FindP(unsigned x)
{
	for (unsigned p = TBase::p[x]; x != p; )
	{
		unsigned pp = TBase::p[p];
		TBase::p[x] = pp;
		x = p;
		p = pp;
	}
	return x;
}
