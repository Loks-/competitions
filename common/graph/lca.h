#pragma once

#include "base_tree.h"
#include <algorithm>
#include <stack>
#include <vector>

// Schieber-Vishkin decomposition
class LCA
{
public:
	unsigned n, n2p;
	std::vector<unsigned> vlbit, vrbit;
	std::vector<unsigned> parent, preorder, subtreesize, I, lead, A;
	std::vector<unsigned> deep;

protected:
	void InitBitMaps()
	{
		for (n2p = 1; n2p < n; n2p <<= 1);
		if (vlbit.size() < n2p)
		{
			vlbit.resize(n2p);
			vrbit.resize(n2p);
			vlbit[0] = vrbit[0] = 0;
			for (unsigned i = 1; i < n2p; i <<= 1)
			{
				fill(vlbit.begin() + i, vlbit.begin() + 2 * i, i);
				copy(vrbit.begin() + 1, vrbit.begin() + i, vrbit.begin() + i + 1);
				vrbit[i] = i;
			}
		}
	}

public:
	LCA() : n(0), n2p(1) {}
	LCA(const BaseTree& g) { Build(g); }

	void Init()
	{
		InitBitMaps();
		parent.resize(n);
		preorder.resize(n);
		subtreesize.resize(n);
		I.resize(n);
		lead.resize(n + 1);
		A.resize(n);
		deep.resize(n);
	}

	void Build(const BaseTree& g)
	{
		n = g.nvertices;
		Init();
		DFS1(g);
		DFS2(g);
	}

protected:
	void DFS1(const BaseTree& g)
	{
		std::stack<unsigned> s;
		std::vector<bool> visited(n, false);
		unsigned timer = 0;
		parent[g.root] = CNone;
		deep[g.root] = 0;
		for (s.push(g.root); !s.empty(); )
		{
			unsigned v = s.top();
			unsigned p = parent[v];
			unsigned d = deep[v];
			if (!visited[v])
			{
				// First time here, add children
				visited[v] = true;
				I[v] = preorder[v] = timer++;
				for (unsigned c : g.edges[v])
				{
					if (c == p) continue; // For undirected edges
					parent[c] = v;
					deep[c] = d + 1;
					s.push(c);
				}
			}
			else
			{
				// Second time here, finalize
				s.pop();
				subtreesize[v] = timer - preorder[v];
				if ((v != g.root) && (vrbit[I[v]] > vrbit[I[p]]))
					I[p] = I[v];
				lead[I[v]] = v;
			}
		}
	}

	void DFS2(const BaseTree& g)
	{
		std::stack<unsigned> s;
		for (s.push(g.root); !s.empty(); )
		{
			unsigned v = s.top(); s.pop();
			A[v] = ((v != g.root) ? A[parent[v]] : 0) | vrbit[I[v]];
			for (unsigned c : g.edges[v])
			{
				// Safety check
				if (c == parent[v]) continue;  // For undirected edges
				s.push(c);
			}
		}
	}

	unsigned EnterIntoStrip(unsigned x, unsigned hz) const
	{
		if (vrbit[I[x]] == hz) return x;
		unsigned hw = vlbit[A[x] & (hz - 1)];
		return parent[lead[I[x] & (~hw + 1) | hw]];
	}

public:
	unsigned GetLCA(unsigned x, unsigned y) const
	{
		unsigned hb = (I[x] == I[y]) ? vrbit[I[x]] : vlbit[I[x] ^ I[y]]; // Some magic
		unsigned hz = vrbit[A[x] & A[y] & (~hb + 1)];
		unsigned ex = EnterIntoStrip(x, hz);
		unsigned ey = EnterIntoStrip(y, hz);
		return preorder[ex] < preorder[ey] ? ex : ey;
	}

	unsigned GetDistance(unsigned x, unsigned y) const
	{
		unsigned z = GetLCA(x, y);
		return deep[x] + deep[y] - 2 * deep[z];
	}
};
