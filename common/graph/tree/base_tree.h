#pragma once

#include "../base_graph.h"
#include <iostream>
#include <vector>

class BaseTree : public BaseUndirectedGraph
{
public:
	using TBase = BaseUndirectedGraph;
	using TSelf = BaseTree;

public:
	unsigned root;

public:
	BaseTree(unsigned _nvertices = 0, unsigned _root = 0) : TBase(_nvertices), root(_root) {}

	void Resize(unsigned _nvertices, unsigned _root = 0)
	{
		TBase::Resize(_nvertices);
		root = _root;
	}

	void ReadEdges(bool zero_based_indexes = false)	{ TBase::ReadEdges(TBase::nvertices - 1, zero_based_indexes); }

	void ReadTreeEdges(bool zero_based_indexes = false)
	{
		unsigned shift = zero_based_indexes ? 0 : 1;
		for (unsigned i = 1; i < TBase::nvertices; ++i)
		{
			unsigned from;
			std::cin >> from;
			TBase::AddEdge(from - shift, i);
		}
	}

	std::vector<unsigned> GetDeep() const { return TBase::GetDistances(root); }
};