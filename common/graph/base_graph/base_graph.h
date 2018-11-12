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

protected:
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
	unsigned Size() const { return nvertices; }
	std::vector<std::vector<unsigned>>& Edges() { return edges; }
	const std::vector<std::vector<unsigned>>& Edges() const { return edges; }
	std::vector<unsigned>& Edges(unsigned from) { return edges[from]; }
	const std::vector<unsigned>& Edges(unsigned from) const { return edges[from]; }
	std::vector<std::vector<unsigned>>& InvertedEdges() { return inverted_edges; }
	const std::vector<std::vector<unsigned>>& InvertedEdges() const { return inverted_edges; }
	std::vector<unsigned>& InvertedEdges(unsigned from) { return inverted_edges[from]; }
	const std::vector<unsigned>& InvertedEdges(unsigned from) const { return inverted_edges[from]; }

	void AddBaseEdge(unsigned from, unsigned to) { edges[from].push_back(to); }
	void AddInvertedEdge(unsigned from, unsigned to) { inverted_edges[from].push_back(to); }
	void AddEdge(unsigned from, unsigned to) { AddBaseEdge(from, to); if (directed_edges) AddInvertedEdge(to, from); else AddBaseEdge(to, from); }

	void ReadEdges(unsigned edges_to_read, bool zero_based_indexes = false)
	{
		unsigned shift = zero_based_indexes ? 0 : 1;
		for (; edges_to_read--;)
		{
			unsigned from, to;
			std::cin >> from >> to;
			AddEdge(from - shift, to - shift);
		}
	}
};

using BaseUndirectedGraph = BaseGraph<false>;
using BaseDirectedGraph = BaseGraph<true>;
