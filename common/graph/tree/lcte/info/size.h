#pragma once

#include "common/base.h"
#include "common/graph/tree/lcte/info/none.h"

namespace graph {
namespace lcte {
namespace info {
class PSize : public PNone {
 public:
  using TBase = PNone;
  using TSelf = PSize;

  size_t csize = 0;
  size_t psize = 0;
  size_t vsize = 0;
  size_t tsize = 0;

  template <class TNode>
  void PUpdate(TNode* node) {
    csize = (node->vc ? node->vc->info.vsize : 0);
    psize = 1;
    vsize = csize;
    if (node->l) {
      psize += node->l->info.psize;
      vsize += node->l->info.vsize;
    }
    if (node->r) {
      psize += node->r->info.psize;
      vsize += node->r->info.vsize;
    }
    tsize = psize + vsize;
  }
};

class VSize : public VNone {
 public:
  using TBase = VNone;
  using TSelf = VSize;

  size_t vsize = 0;

  template <class TNode>
  void VUpdate(TNode* node) {
    vsize = node->vc->info.tsize + (node->l ? node->l->info.vsize : 0) +
            (node->r ? node->r->info.vsize : 0);
  }
};
}  // namespace info
}  // namespace lcte
}  // namespace graph
