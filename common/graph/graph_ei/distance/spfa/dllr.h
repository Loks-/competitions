#pragma once

#include "common/base.h"
#include "common/node.h"
#include "common/nodes_manager_fixed_size.h"

namespace graph {
namespace distance {
namespace spfa {
// Double linked list representation for childs for Tarjan and ZDO algorithms.
class DLLRNode : public BaseNode {
 public:
  DLLRNode *l = nullptr, *r = nullptr, *p = nullptr;
  unsigned nchilds = 0;

  void Reset() {
    nchilds = 0;
    p = nullptr;
  }
};

class DLLR : public NodesManagerFixedSize<DLLRNode> {
 public:
  using TNode = DLLRNode;
  using TBase = NodesManagerFixedSize<TNode>;

  DLLR(unsigned size) : TBase(size) {}

  TNode* Get(unsigned index) { return TBase::NodeByRawIndex(index); }
  unsigned Get(const TNode* node) { return TBase::RawIndex(node); }

  void SetRoot(unsigned r) {
    auto n = Get(r);
    n->l = n->r = n;
    n->nchilds = 0;
  }

  void SetParent(unsigned c, unsigned p) {
    auto nc = Get(c), np = Get(p);
    assert(nc->nchilds == 0);
    nc->r = np->r;
    nc->r->l = nc;
    nc->l = np;
    np->r = nc;
    nc->p = np;
    ++np->nchilds;
  }
};
}  // namespace spfa
}  // namespace distance
}  // namespace graph
