#pragma once

#include "common/graph/tree/lcte/info/none.h"

namespace graph {
namespace lcte {
namespace info {
template <class TSumType, class TPInfo = PNone>
class PSum : public TPInfo {
 public:
  using TBase = TPInfo;
  using TSelf = PSum<TSumType, TPInfo>;

  TSumType csum;
  TSumType psum;
  TSumType vsum;
  TSumType tsum;

  template <class TNode>
  void PUpdate(TNode* node) {
    TBase::PUpdate(node);
    csum = (node->vc ? node->vc->info.vsum : TSumType());
    psum = node->data;
    vsum = csum;
    if (node->l) {
      psum += node->l->info.psize;
      vsum += node->l->info.vsum;
    }
    if (node->r) {
      psum += node->r->info.psum;
      vsum += node->r->info.vsum;
    }
    tsum = psum + vsum;
  }
};

template <class TSumType, class TVInfo = VNone>
class VSum : public TVInfo {
 public:
  using TBase = TVInfo;
  using TSelf = VSum<TSumType, TPInfo>;

  TSumType vsum;

  template <class TNode>
  void VUpdate(TNode* node) {
    TBase::VUpdate(node);
    vsum = node->vc->info.tsum + (node->l ? node->l->info.vsum : 0) +
           (node->r ? node->r->info.vsum : 0);
  }
};
}  // namespace info
}  // namespace lcte
}  // namespace graph
