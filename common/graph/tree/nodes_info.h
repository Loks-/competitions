#pragma once

#include "common/graph/tree.h"
#include <algorithm>
#include <stack>
#include <vector>

namespace graph {
class TreeNodesInfo {
 public:
  unsigned root;
  std::vector<unsigned> parent, deep, preorder, subtree_size;

  template <class TTree>
  void Init(const TTree& tree, unsigned _root) {
    root = _root;
    unsigned size = tree.Size(), timer = 0;
    parent.resize(size);
    deep.resize(size);
    preorder.resize(size);
    subtree_size.resize(size);
    std::fill(subtree_size.begin(), subtree_size.end(), 0);
    parent[root] = CNone;
    deep[root] = 0;
    std::stack<unsigned> s;
    for (s.push(root); !s.empty();) {
      unsigned v = s.top(), p = parent[v], d = deep[v];
      if (subtree_size[v] == 0) {
        // First time here
        preorder[v] = timer++;
        subtree_size[v] = 1;
        for (unsigned c : tree.Edges(v)) {
          if (c == p) continue;
          parent[c] = v;
          deep[c] = d + 1;
          s.push(c);
        }
      } else {
        s.pop();
        subtree_size[v] = timer - preorder[v];
      }
    }
  }

  template <class TTree>
  void Init(const TTree& tree) {
    Init(tree, tree.GetRoot());
  }

  TreeNodesInfo() : root(CNone) {}

  template <class TTree>
  TreeNodesInfo(const TTree& tree) {
    Init(tree);
  }

  template <class TTree>
  TreeNodesInfo(const TTree& tree, unsigned _root) {
    Init(tree, _root);
  }
};
}  // namespace graph
