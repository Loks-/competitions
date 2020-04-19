#pragma once

#include "common/graph/tree_ei.h"

#include <algorithm>
#include <stack>
#include <vector>

namespace graph {
// Time: O(V)
template <class TTEdgeInfo>
class TreeEINodesInfo {
 public:
  using TEdgeInfo = TTEdgeInfo;

 public:
  unsigned root;
  std::vector<unsigned> parent, deep, preorder, subtree_size;
  std::vector<TEdgeInfo> distance_to_root;

  void Init(const TreeEI<TEdgeInfo>& tree, unsigned _root) {
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
    for (s.push(root); !s.empty();) {
      unsigned v = s.top(), p = parent[v], d = deep[v];
      TEdgeInfo dr = distance_to_root[v];
      if (subtree_size[v] == 0) {
        // First time here
        preorder[v] = timer++;
        subtree_size[v] = 1;
        for (auto ce : tree.EdgesEI(v)) {
          unsigned c = ce.to;
          if (c == p) continue;
          parent[c] = v;
          deep[c] = d + 1;
          distance_to_root[c] = dr + ce.info;
          s.push(c);
        }
      } else {
        s.pop();
        subtree_size[v] = timer - preorder[v];
      }
    }
  }

  void Init(const TreeEI<TEdgeInfo>& tree) { Init(tree, tree.GetRoot()); }

  TreeEINodesInfo() : root(CNone) {}
  explicit TreeEINodesInfo(const TreeEI<TEdgeInfo>& tree) { Init(tree); }
  TreeEINodesInfo(const TreeEI<TEdgeInfo>& tree, unsigned _root) {
    Init(tree, _root);
  }
};
}  // namespace graph
