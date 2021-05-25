#pragma once

#include "common/graph/cnone.h"

#include <vector>

namespace graph {
// Forest with parent-childs representations and O(1) updating parent
class PCR {
 public:
  class Node {
   public:
    unsigned parent = CNone;
    unsigned child_index = 0;
    std::vector<unsigned> childs;
  };

  std::vector<Node> nodes;

 public:
  void Reset(unsigned size) {
    nodes.clear();
    nodes.resize(size);
  }

  PCR() {}
  PCR(unsigned size) { Reset(size); }

  unsigned Size() const { return nodes.size(); }

  void ClearP(unsigned c) {
    auto& n = nodes[c];
    if (n.parent == CNone) return;
    auto& p = nodes[n.parent];
    if (n.child_index + 1 < p.childs.size()) {
      auto c2 = p.childs.back();
      p.childs[n.child_index] = c2;
      nodes[c2].child_index = n.child_index;
    }
    p.childs.pop_back();
    n.parent = CNone;
  }

  void SetP(unsigned c, unsigned p) {
    auto& cn = nodes[c];
    if (cn.parent == p) return;
    ClearP(c);
    if (p == CNone) return;
    auto& pn = nodes[p];
    cn.parent = p;
    cn.child_index = pn.childs.size();
    pn.childs.push_back(c);
  }
};
}  // namespace graph
