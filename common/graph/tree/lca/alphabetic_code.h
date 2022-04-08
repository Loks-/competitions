#pragma once

#include "common/graph/tree.h"
#include "common/numeric/alphabetic_code.h"
#include "common/numeric/bits/first_bit.h"
#include "common/numeric/bits/parity.h"
#include "common/numeric/bits/ulog2.h"

#include <stack>
#include <unordered_map>
#include <vector>

#include <iostream>

namespace graph {
namespace lca {
// First version for LCA using Alphabetic Code.
// TODO:
//   1. Better compression for codes (we use 59~63 bits for n = 1M)
//   2. Invert codes
// Time: O(V) preprocessing, O(1) lca
class AlphabeticCode {
 public:
  using TCode = numeric::AlphabeticCode<uint64_t, unsigned>;

 protected:
  unsigned root;
  std::vector<TCode> vl, vk;
  std::unordered_map<uint64_t, unsigned> mc2v;

 public:
  AlphabeticCode() {}

  template <class TGraph>
  explicit AlphabeticCode(const Tree<TGraph>& g) {
    Build(g);
  }

  void Init(unsigned n) {
    vl.resize(n);
    vk.resize(n);
    mc2v.clear();
  }

  template <class TGraph>
  void Build(const Tree<TGraph>& g) {
    unsigned n = g.Size();
    Init(n);
    root = g.GetRoot();
    std::vector<unsigned> vs(n, 1), vp(n, CNone), va(n, CNone), visited(n, 0),
        vpath1, vpath2;
    std::vector<TCode> vhl(n), vll(n);
    std::stack<unsigned> s;
    for (s.push(root); !s.empty();) {
      unsigned v = s.top(), p = vp[v];
      if (visited[v]++) {
        s.pop();
        for (unsigned u : g.Edges(v)) {
          if (u != p) vs[v] += vs[u];
        }
      } else {
        for (unsigned u : g.Edges(v)) {
          if (u != p) {
            s.push(u);
            vp[u] = v;
          }
        }
      }
    }
    for (s.push(root); !s.empty();) {
      unsigned v = s.top(), p = vp[v];
      s.pop();
      vpath1.clear();
      vpath2.clear();
      vpath2.push_back(1);
      for (unsigned v0 = v, v0p = p, v1; v0 != CNone; v0p = v0, v0 = v1) {
        va[v0] = v;
        v1 = CNone;
        unsigned ms = 0;
        for (unsigned u : g.Edges(v0)) {
          if (u == v0p) continue;
          if (ms < vs[u]) {
            ms = vs[u];
            v1 = u;
          }
        }
        for (unsigned u : g.Edges(v0)) {
          if ((u != v0p) && (u != v1)) s.push(u);
        }
        vpath1.push_back(v0);
        vpath2.push_back(vs[v0] - ms);
      }
      auto vpc = TCode::Construct(vpath2);
      for (unsigned i = 0; i < vpath1.size(); ++i) vhl[vpath1[i]] = vpc[i + 1];
    }
    vl[root] = vll[root] + vhl[root];
    vk[root] = TCode(1, vl[root].length);
    for (s.push(root); !s.empty();) {
      unsigned v = s.top(), p = vp[v],
               lc = g.Edges(v).size() + ((p == CNone) ? 1 : 0) - 2,
               lcl = numeric::ULog2(lc - 1) + 1, li = 0;
      s.pop();
      for (unsigned u : g.Edges(v)) {
        if (u == p) continue;
        s.push(u);
        unsigned ua = va[u], uap = vp[ua];
        if (ua == u) vll[u] = TCode(li++, lcl);
        vl[u] = (uap == CNone) ? vhl[u] : vl[uap] + vll[ua] + vhl[u];
        vk[u] = (uap == CNone) ? TCode(1, vhl[u].length)
                               : vk[uap] + TCode(1, vll[ua].length) +
                                     TCode(1, vhl[u].length);
      }
    }
    // unsigned mk = 0;
    // for (auto c : vl) mk = std::max(mk, c.length);
    // std::cout << "Max length = " << mk << std::endl;

    for (unsigned i = 0; i < n; ++i) mc2v[vl[i].Adjust()] = i;
  }

 public:
  unsigned GetLCA(unsigned x, unsigned y) const {
    if (x == y) return x;
    uint64_t lx = vl[x].Adjust(), ly = vl[y].Adjust(), ld = lx ^ ly, kx;
    if (ly < lx) {
      lx = ly;
      kx = vk[y].Adjust();
    } else {
      kx = vk[x].Adjust();
    }
    uint64_t ml = ~((uint64_t(1) << (numeric::HighestBit(ld | 1) + 1)) - 1),
             kt = kx & ml;
    if (!numeric::BitsParity(kt)) {
      uint64_t kx0 = kx ^ kt;
      kt |= (uint64_t(1) << numeric::HighestBit(kx0));
    }
    uint64_t km = ~((kt ^ (kt - 1)) >> 1), l = lx & km;
    auto it = mc2v.find(l);
    assert(it != mc2v.end());
    return it->second;
  }
};
}  // namespace lca
}  // namespace graph
