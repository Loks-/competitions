#pragma once

#include "base_tree.h"
#include "../../base.h"
#include <algorithm>
#include <stack>
#include <vector>

class ICentroidDecompositionCallBack
{
public:
	void Decompose(unsigned vertex, const std::vector<unsigned>& group) {}
	bool IsComposeRequired() const { return false; }
	void Compose(unsigned vertex, const std::vector<unsigned>& group) {}
};

class CentroidDecomposition
{
protected:
	const BaseTree& tree;
	std::vector<unsigned> group, parent, subtree_size;
	std::vector<unsigned> group_root;
	std::stack<unsigned> vertices_to_check;

	CentroidDecomposition(const BaseTree& _tree) : tree(_tree)
	{
		unsigned n = tree.nvertices;
		group.resize(n, 0);
		subtree_size.resize(n, 0);
		parent.resize(n);
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
		unsigned best_size = group_size, best_node = root;
		unsigned v = root;
		for (bool next = true; next; )
		{
			next = false;
			unsigned p = parent[v];
			for (unsigned c : tree.edges[v])
			{
				if (c == p) continue;
				if (group[c] != current_group) continue;
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
		for (unsigned v : tree.edges[root])
		{
			if (group[v] != current_group) continue;
			unsigned new_group = unsigned(group_root.size());
			group_root.push_back(v);
			for (vertices_to_check.push(v); !vertices_to_check.empty();)
			{
				v = vertices_to_check.top(); vertices_to_check.pop();
				group[v] = new_group;
				for (unsigned c : tree.edges[v])
				{
					if (c == root) continue;
					if (group[c] == current_group)
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
			cd.Decompose(i);
			callback.Decompose(cd.group_root[i], cd.group);
		}
		if (callback.IsComposeRequired())
		{
			// ...
			assert(false);
		}
	}
};
