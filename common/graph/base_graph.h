#pragma once

#include <iostream>
#include <queue>
#include <vector>

const static unsigned CNone = unsigned(-1);

template <bool _directed_edges = false>
class BaseGraph
{
public:
	const static bool directed_edges = _directed_edges;
	using TSelf = BaseGraph<directed_edges>;

public:
	unsigned nvertices;
	std::vector<std::vector<unsigned>> edges;
	std::vector<std::vector<unsigned>> inverted_edges;

public:
	void Clear()
	{
		nvertices = 0;
		edges.clear();
		inverted_edges.clear();
	}

	void Resize(unsigned _nvertices)
	{
		Clear();
		nvertices = _nvertices;
		edges.resize(nvertices);
		if (directed_edges)
			inverted_edges.resize(nvertices);
	}

	BaseGraph(unsigned _nvertices = 0) { Resize(_nvertices); }

	void AddEdge(unsigned from, unsigned to)
	{
		edges[from].push_back(to);
		if (directed_edges)
			inverted_edges[to].push_back(from);
		else
			edges[to].push_back(from);
	}

	void ReadEdges(unsigned edges_to_read, bool zero_based_indexes = false)
	{
		unsigned shift = zero_based_indexes ? 0 : 1;
		for (; edges_to_read; --edges_to_read)
		{
			unsigned from, to;
			std::cin >> from >> to;
			AddEdge(from - shift, to - shift);
		}
	}

	// Return distance to each vertex from required vertex.
	// If some vertex is unreachable it returns -1 for it.
	std::vector<unsigned> GetDistances(unsigned source) const
	{
		const unsigned none = -1;
		std::vector<unsigned> d(nvertices, none);
		d[source] = 0;
		std::queue<unsigned> q;
		q.push(source);
		while (!q.empty())
		{
			unsigned f = q.front(); q.pop();
			unsigned l = d[f];
			for (size_t i = 0; i < edges[f].size(); ++i)
			{
				unsigned k = edges[f][i];
				if (d[k] == none)
				{
					d[k] = l + 1;
					q.push(k);
				}
			}
		}
		return d;
	}
};

using BaseUndirectedGraph = BaseGraph<false>;
using BaseDirectedGraph = BaseGraph<true>;
