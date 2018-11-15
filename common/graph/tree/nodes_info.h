#pragma once

#include "tree.h"
#include <algorithm>
#include <stack>
#include <vector>

template<class TTEdgeInfo>
class TreeNodesInfo
{
public:
    using TEdgeInfo = TTEdgeInfo;

public:
    unsigned root;
    std::vector<unsigned> parent, deep, preorder, subtree_size;
    std::vector<TEdgeInfo> distance_to_root;

    void Init(const Tree<TEdgeInfo>& tree, unsigned _root)
    {
        root = _root;
        unsigned size = tree.Size(), timer = 0;
        parent.resize(size);
        deep.resize(size);
        preorder.resize(size);
        subtree_size.resize(size);
        distance_to_root.resize(size);
        std::fill(subtree_size.begin(), subtree_size.end(), 0);
        parent[root] = CNone;
        deep[root] = 0;
        distance_to_root[root] = 0;
        std::stack<unsigned> s;
        for (s.push(root); !s.empty(); )
        {
            unsigned v = s.top(), p = parent[v], d = deep[v];
            TEdgeInfo dr = distance_to_root[v];
            if (subtree_size[v] == 0)
            {
                // First time here
                preorder[v] = timer++;
                subtree_size[v] = 1;
        		for (auto ce : tree.GraphEdges(v))
				{
                    unsigned c = ce.to;
					if (c == p) continue;
					parent[c] = v;
					deep[c] = d + 1;
                    distance_to_root[c] = dr + ce.info;
					s.push(c);
				}
            }
            else
            {
                s.pop();
                subtree_size[v] = timer - preorder[v];
            }
        }
    }

    void Init(const Tree<TEdgeInfo>& tree) { Init(tree, tree.GetRoot()); }
    TreeNodesInfo() : root(CNone) {}
    TreeNodesInfo(const Tree<TEdgeInfo>& tree) { Init(tree); }
    TreeNodesInfo(const Tree<TEdgeInfo>& tree, unsigned _root) { Init(tree, _root); }
};
