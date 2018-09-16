#pragma once

#include "base_graph.h"
#include "base_tree.h"
#include "../base.h"
#include <vector>

class BaseSubgraph
{
protected:
	template<class TBaseGraph>
	static void UpdateRoot(const TBaseGraph& graph, unsigned new_nvertices, const std::vector<unsigned>& old_to_new, TBaseGraph& output) {}
	static void UpdateRoot(const BaseTree& graph, unsigned new_nvertices, const std::vector<unsigned>& old_to_new, BaseTree& output)
	{
		unsigned root_new = old_to_new[graph.root];
		output.root = (root_new >= new_nvertices) ? 0 : root_new;
	}

public:
	template<class TBaseGraph>
	static void CreateSubgraph(const TBaseGraph& graph, unsigned new_nvertices, const std::vector<unsigned>& old_to_new, TBaseGraph& output)
	{
		assert(graph.nvertices == old_to_new.size());
		output.Resize(new_nvertices);
		for (unsigned u = 0; u < graph.nvertices; ++u)
		{
			unsigned unew = old_to_new[u];
			if (unew >= new_nvertices) continue;
			for (unsigned v : graph.edges[u])
			{
				unsigned vnew = old_to_new[v];
				if (vnew >= new_nvertices) continue;
				output.edges[unew].push_back(vnew);
			}
			if (graph.directed_edges)
			{
				for (unsigned v : graph.inverted_edges[u])
				{
					unsigned vnew = old_to_new[v];
					if (vnew >= new_nvertices) continue;
					output.inverted_edges[unew].push_back(vnew);
				}
			}
		}
		UpdateRoot(graph, new_nvertices, old_to_new, output);
	}

	template<class TBaseGraph>
	static void CreateSubgraph(const TBaseGraph& graph, const std::vector<bool>& keep, TBaseGraph& output)
	{
		assert(graph.nvertices == keep.size());
		unsigned new_nvertices = 0;
		std::vector<unsigned> old_to_new(graph.nvertices, CNone);
		for (unsigned i = 0; i < graph.nvertices; ++i)
		{
			if (keep[i])
				old_to_new[i] = new_nvertices++;
		}
		CreateSubgraph(graph, new_nvertices, old_to_new, output);
	}
};
