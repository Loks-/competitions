#pragma once

#include "../base.h"
#include "base_tree.h"
#include "../hash.h"
#include <algorithm>
#include <functional>
#include <stack>
#include <vector>
#include <utility>

class BaseTreeIsomorphicHash
{
protected:
	static std::pair<unsigned, unsigned> GetCenter(const BaseTree& tree)
	{
		struct Node
		{
			unsigned node, parent;
		};
		unsigned n = tree.nvertices;
		std::vector<unsigned> subtree_size(n,  0);
		std::stack<Node> s;
		for (s.push({ tree.root, CNone }); !s.empty(); )
		{
			unsigned v = s.top().node;
			unsigned p = s.top().parent;
			if (subtree_size[v] == 0)
			{
				// First time here, add children
				subtree_size[v] = 1;
				for (unsigned c : tree.edges[v])
				{
					if (c == p) continue;
					s.push({ c, v });
				}
			}
			else
			{
				// Second time here, finalize
				s.pop();
				for (unsigned c : tree.edges[v])
				{
					if (c == p) continue;
					subtree_size[v] += subtree_size[c];
				}
				if (2 * subtree_size[v] == n)
					return std::make_pair(v, p);				
				if (2 * subtree_size[v] > n)
					return std::make_pair(v, CNone);
			}
		}
		assert(false);
		return std::make_pair(tree.root, CNone);
	}

	static size_t HashR(const BaseTree& tree, unsigned node, unsigned parent)
	{
		size_t current = std::hash<unsigned>{}(1);
		std::vector<size_t> v;
		for (unsigned c : tree.edges[node])
		{
			if (c == parent) continue;
			v.push_back(HashR(tree, c, node));
		}
		std::sort(v.begin(), v.end());
		for (size_t h : v)
			current = std::hash_combine(current, h);
		return current;
	}

public:
	static size_t Hash(const BaseTree& tree)
	{
		auto p = GetCenter(tree);
		if (p.second == CNone) return HashR(tree, p.first, p.second);
		size_t h1 = HashR(tree, p.first, p.second);
		size_t h2 = HashR(tree, p.second, p.first);
		return (h1 < h2) ? std::hash_combine(h1, h2) : std::hash_combine(h2, h1);
	}
};
