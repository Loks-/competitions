#pragma once

#include "common/graph/graph.h"
#include "common/graph/graph/adjacency_matrix.h"
#include "common/vector/unsigned_set.h"
#include <stack>
#include <vector>

#include "common/vector/write.h"

namespace graph {
// Bron–Kerbosch algorithm
// https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
// Callback function will be called with one parameter - UnsignedSet that
// contains maximal clique.
// TODO:
//   1. Use degeneracy ordering on first step.
class MaximalCliquesList {
 protected:
  const UndirectedGraph& g;
  std::vector<std::vector<bool>> gam;
  unsigned n;

 protected:
  UnsignedSet r, p, x;
  std::stack<unsigned> sr, sp, sx;

 public:
  MaximalCliquesList(const UndirectedGraph& _g)
      : g(_g), n(g.Size()), r(n), p(n), x(n) {
    gam = AdjacencyMatrix(g);
  }

 protected:
  void UpdatePivot(unsigned node, unsigned& pivot, unsigned& best_size) {
    unsigned current_size = 0;
    for (unsigned v : g.Edges(node)) {
      if (p.HasKey(v)) ++current_size;
    }
    if (best_size < current_size) {
      best_size = current_size;
      pivot = node;
    }
  }

  template <class TCallbackFunction>
  void SearchI(TCallbackFunction& f) {
    // std::cout << "SearchI" << std::endl;
    // std::cout << "r = ";
    // WriteVector(r.vlist);
    // std::cout << "p = ";
    // WriteVector(p.vlist);
    // std::cout << "x = ";
    // WriteVector(x.vlist);

    if (p.Empty() && x.Empty()) f(r);
    if (p.Empty()) return;
    // Find pivot
    unsigned pivot = p.vlist[0], best_size = 0;
    for (unsigned node : p.vlist) UpdatePivot(node, pivot, best_size);
    for (unsigned node : x.vlist) UpdatePivot(node, pivot, best_size);
    // Run recursion
    unsigned srs = sr.size(), sps = sp.size(), sxs = sx.size();
    for (unsigned node = 0; node < n; ++node) {
      if (!p.HasKey(node) || gam[pivot][node]) continue;
      r.Insert(node);
      for (unsigned u = 0; u < n; ++u) {
        if (gam[node][u]) continue;
        if (p.HasKey(u)) {
          p.Remove(u);
          sp.push(u);
        }
        if (x.HasKey(u)) {
          x.Remove(u);
          sx.push(u);
        }
      }
      SearchI(f);
      r.RemoveLast();
      for (; sp.size() > sps; sp.pop()) p.Insert(sp.top());
      for (; sx.size() > sxs; sx.pop()) x.Insert(sx.top());
      p.Remove(node);
      x.Insert(node);
      sr.push(node);
    }
    // Revert changes
    for (; sr.size() > srs; sr.pop()) {
      unsigned u = sr.top();
      p.Insert(u);
      x.Remove(u);
    }
  }

 public:
  template <class TCallbackFunction>
  void Search(TCallbackFunction& f) {
    for (unsigned i = 0; i < n; ++i) p.Insert(i);
    r.Clear();
    x.Clear();
    SearchI(f);
  }
};
}  // namespace graph
