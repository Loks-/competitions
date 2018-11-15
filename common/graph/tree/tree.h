#pragma once

#include "../base_tree/base_tree.h"
#include "../graph/graph.h"
#include <iostream>
#include <vector>

template <class TTEdgeInfo>
class Tree : public BaseTree<UndirectedGraph<TTEdgeInfo>>
{
public:
	using TBaseTree = BaseTree<UndirectedGraph<TTEdgeInfo>>;
	using TGraph = typename TBaseTree::TBase;
    using TBaseGraph = typename TGraph::TBase;
    using TEdgeInfo = typename TGraph::TEdgeInfo;
    using TSelf = Tree<TTEdgeInfo>;

public:
	Tree(unsigned _nvertices = 0, unsigned _root = 0) : TBaseTree(_nvertices, _root) {}

	void ReadEdges(bool zero_based_indexes = false)	{ TGraph::ReadEdges(TBaseGraph::nvertices - 1, zero_based_indexes); }

	void ReadTreeEdges(bool zero_based_indexes = false)
	{
		unsigned shift = zero_based_indexes ? 0 : 1;
		for (unsigned i = 1; i < TBaseGraph::nvertices; ++i)
		{
            TEdgeInfo edge_info;
			unsigned from;
			std::cin >> from >> edge_info;
			TGraph::AddEdge(from - shift, i, edge_info);
		}
	}
};
