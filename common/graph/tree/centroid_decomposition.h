#pragma once

#include "base_tree.h"
#include "../../base.h"
#include <algorithm>
#include <stack>
#include <vector>

class CentroidDecomposition
{
protected:
	// Edges from smaller groups to parent are removed.
	BaseTree tree;
	std::vector<unsigned> group, parent, subtree_size, group_root;
	std::stack<unsigned> vertices_to_check;

public:
	const BaseTree& GetCurrentTree() const { return tree; }
	const std::vector<unsigned>& GetGroupMap() const { return group; }

protected:
	CentroidDecomposition(const BaseTree& _tree) : tree(_tree)
	{
		unsigned n = tree.nvertices;
		group.resize(n, 0);
		parent.resize(n);
		subtree_size.resize(n, 0);
		group_root.reserve(n);
	}

	void InitParentAndSubtreeSizes()
	{
		std::fill(subtree_size.begin(), subtree_size.end(), 0);
		parent[tree.root] = CNone;
		group_root.clear();
		group_root.push_back(tree.root);
		for (vertices_to_check.push(tree.root); !vertices_to_check.empty(); )
		{
			unsigned v = vertices_to_check.top();
			unsigned p = parent[v];
			if (subtree_size[v] == 0)
			{
				// First time here, add children
				subtree_size[v] = 1;
				for (unsigned c : tree.edges[v])
				{
					if (c == p) continue;
					parent[c] = v;
					vertices_to_check.push(c);
				}
			}
			else
			{
				// Second time here, finalize
				vertices_to_check.pop();
				for (unsigned c : tree.edges[v])
				{
					if (c == p) continue;
					subtree_size[v] += subtree_size[c];
				}
			}
		}
	}

	void FindCentroid(unsigned current_group)
	{
		unsigned root = group_root[current_group];
		unsigned group_size = subtree_size[root];
		if (group_size <= 2) return;
		unsigned best_size = group_size, best_node = root;
		unsigned v = root;
		for (bool next = true; next; )
		{
			next = false;
			unsigned p = parent[v];
			for (unsigned c : tree.edges[v])
			{
				if (c == p) continue;
				if (subtree_size[c] > group_size / 2)
				{
					v = c;
					next = true;
					break;
				}
			}
		}
		if (v != root)
		{
			// Update parent and subtree_size with new root in centroid
			unsigned vparent = parent[v];
			unsigned vsize = subtree_size[v];
			group_root[current_group] = v;
			parent[v] = parent[root];
			subtree_size[v] = group_size;
			for (; v != root; )
			{
				unsigned vnextparent = parent[vparent];
				parent[vparent] = v;
				v = vparent; vparent = vnextparent;
				unsigned vnextsize = subtree_size[v];
				subtree_size[v] = group_size - vsize;
				vsize = vnextsize;
			}
		}
	}

	void Decompose(unsigned current_group)
	{
		unsigned root = group_root[current_group];
		if (subtree_size[root] == 1) return;
		for (unsigned v : tree.edges[root])
		{
			unsigned new_group = unsigned(group_root.size());
			group_root.push_back(v);
			group[v] = new_group;
			std::vector<unsigned>& vedges = tree.edges[v];
			for (unsigned i = 0; i < vedges.size(); )
			{
				if (vedges[i] == root)
				{
					std::swap(vedges[i], vedges.back());
					vedges.pop_back();
				}
				else
					vertices_to_check.push(vedges[i++]);
			}
			for (; !vertices_to_check.empty();)
			{
				unsigned u = vertices_to_check.top(); vertices_to_check.pop();
				unsigned p = parent[u];
				group[u] = new_group;
				for (unsigned c : tree.edges[u])
				{
					if (c == p) continue;
					vertices_to_check.push(c);
				}
			}
		}
	}

public:
	template<class TCentroidDecompositionCallBack>
	static void Run(const BaseTree& tree, TCentroidDecompositionCallBack& callback)
	{
		CentroidDecomposition cd(tree);
		cd.InitParentAndSubtreeSizes();
		for (unsigned i = 0; i < cd.group_root.size(); ++i)
		{
			cd.FindCentroid(i);
			callback.PreDecompose(cd.group_root[i], cd);
			cd.Decompose(i);
			callback.PostDecompose(cd.group_root[i], cd);
		}
		if (callback.IsComposeRequired())
		{
			// ...
			assert(false);
		}
	}
};

class ICentroidDecompositionCallBack
{
public:
	void PreDecompose(unsigned vertex, const CentroidDecomposition& cd) {}
	void PostDecompose(unsigned vertex, const CentroidDecomposition& cd) {}
	bool IsComposeRequired() const { return false; }
	void Compose(unsigned vertex, const CentroidDecomposition& cd) {}
};
