#pragma once

#include "common/base.h"
#include "common/graph/tree.h"
#include "common/hash/combine.h"

#include <algorithm>
#include <functional>
#include <stack>
#include <utility>
#include <vector>

namespace graph {
// Time: O(V log V)
class TreeIsomorphicHash {
 protected:
  static std::pair<unsigned, unsigned> GetCenter(const TreeGraph& tree) {
    struct Node {
      unsigned node, parent;
    };
    unsigned n = tree.Size(), timer = 1;
    std::vector<unsigned> vtimer(n, 0);
    std::stack<Node> s;
    for (s.push({tree.GetRoot(), CNone}); !s.empty();) {
      unsigned v = s.top().node;
      unsigned p = s.top().parent;
      if (vtimer[v] == 0) {
        // First time here, add children
        vtimer[v] = timer++;
        for (unsigned c : tree.Edges(v)) {
          if (c == p) continue;
          s.push({c, v});
        }
      } else {
        // Second time here, finalize
        s.pop();
        unsigned subtree_size = timer - vtimer[v];
        if (2 * subtree_size == n) return std::make_pair(v, p);
        if (2 * subtree_size > n) return std::make_pair(v, CNone);
      }
    }
    assert(false);
    return std::make_pair(tree.GetRoot(), CNone);
  }

  static size_t HashR(const TreeGraph& tree, unsigned node, unsigned parent) {
    size_t seed = 1;
    std::vector<size_t> v;
    for (unsigned c : tree.Edges(node)) {
      if (c == parent) continue;
      v.push_back(HashR(tree, c, node));
    }
    std::sort(v.begin(), v.end());
    for (size_t h : v) nhash::DCombineH(seed, h);
    return seed;
  }

 public:
  static size_t Hash(const TreeGraph& tree) {
    auto p = GetCenter(tree);
    if (p.second == CNone) return HashR(tree, p.first, p.second);
    size_t seed = 0, h1 = HashR(tree, p.first, p.second),
           h2 = HashR(tree, p.second, p.first);
    if (h1 > h2) std::swap(h1, h2);
    nhash::DCombineH(seed, h1);
    nhash::DCombineH(seed, h2);
    return seed;
  }
};
}  // namespace graph

inline size_t IsomorphicHash(const TreeGraph& tree) {
  return graph::TreeIsomorphicHash::Hash(tree);
}
