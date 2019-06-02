#pragma once

#include "common/graph/tree.h"
#include <algorithm>
#include <stack>
#include <vector>

namespace graph {
// Schieber-Vishkin decomposition
// Time: O(V) preprocessing, O(1) lca
class LCA {
 public:
  unsigned n, n2p;
  std::vector<unsigned> vlbit, vrbit;
  std::vector<unsigned> parent, preorder, I, lead, A;
  std::vector<unsigned> deep;

 protected:
  void InitBitMaps() {
    for (n2p = 1; n2p < n;) n2p <<= 1;
    if (vlbit.size() < n2p) {
      vlbit.resize(n2p);
      vrbit.resize(n2p);
      vlbit[0] = vrbit[0] = 0;
      for (unsigned i = 1; i < n2p; i <<= 1) {
        fill(vlbit.begin() + i, vlbit.begin() + 2 * i, i);
        copy(vrbit.begin() + 1, vrbit.begin() + i, vrbit.begin() + i + 1);
        vrbit[i] = i;
      }
    }
  }

 public:
  LCA() : n(0), n2p(1) {}

  template <class TGraph>
  LCA(const Tree<TGraph>& g) {
    Build(g);
  }

  void Init() {
    InitBitMaps();
    parent.resize(n);
    preorder.resize(n);
    I.resize(n);
    lead.resize(n + 1);
    A.resize(n);
    deep.resize(n);
  }

  template <class TGraph>
  void Build(const Tree<TGraph>& g) {
    n = g.Size();
    Init();
    DFS1(g);
    DFS2(g);
  }

 protected:
  template <class TGraph>
  void DFS1(const Tree<TGraph>& g) {
    std::stack<unsigned> s;
    std::vector<bool> visited(n, false);
    unsigned timer = 0;
    unsigned root = g.GetRoot();
    parent[root] = CNone;
    deep[root] = 0;
    for (s.push(root); !s.empty();) {
      unsigned v = s.top();
      unsigned p = parent[v];
      unsigned d = deep[v];
      if (!visited[v]) {
        // First time here, add children
        visited[v] = true;
        I[v] = preorder[v] = timer++;
        for (unsigned c : g.Edges(v)) {
          if (c == p) continue;
          parent[c] = v;
          deep[c] = d + 1;
          s.push(c);
        }
      } else {
        // Second time here, finalize
        s.pop();
        if ((v != root) && (vrbit[I[v]] > vrbit[I[p]])) I[p] = I[v];
        lead[I[v]] = v;
      }
    }
  }

  template <class TGraph>
  void DFS2(const Tree<TGraph>& g) {
    std::stack<unsigned> s;
    for (s.push(g.GetRoot()); !s.empty();) {
      unsigned v = s.top();
      s.pop();
      A[v] = ((v != g.GetRoot()) ? A[parent[v]] : 0) | vrbit[I[v]];
      for (unsigned c : g.Edges(v)) {
        // Safety check
        if (c == parent[v]) continue;
        s.push(c);
      }
    }
  }

  unsigned EnterIntoStrip(unsigned x, unsigned hz) const {
    if (vrbit[I[x]] == hz) return x;
    unsigned hw = vlbit[A[x] & (hz - 1)];
    return parent[lead[I[x] & (~hw + 1) | hw]];
  }

 public:
  unsigned GetLCA(unsigned x, unsigned y) const {
    unsigned hb =
        (I[x] == I[y]) ? vrbit[I[x]] : vlbit[I[x] ^ I[y]];  // Some magic
    unsigned hz = vrbit[A[x] & A[y] & (~hb + 1)];
    unsigned ex = EnterIntoStrip(x, hz);
    unsigned ey = EnterIntoStrip(y, hz);
    return preorder[ex] < preorder[ey] ? ex : ey;
  }

  unsigned GetDistance(unsigned x, unsigned y) const {
    unsigned z = GetLCA(x, y);
    return deep[x] + deep[y] - 2 * deep[z];
  }
};
}  // namespace graph
