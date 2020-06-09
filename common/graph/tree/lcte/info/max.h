#pragma once

#include "common/graph/tree/lcte/info/none.h"

#include <algorithm>

namespace graph {
namespace lcte {
namespace info {
template <class TMaxType, class TPInfo = PNone>
class PMax : public TPInfo {
 public:
  using TBase = TPInfo;
  using TSelf = PMax<TMaxType, TPInfo>;

  bool cmax_set;
  bool vmax_set;
  TMaxType cmax;
  TMaxType pmax;
  TMaxType vmax;
  TMaxType tmax;

  template <class TNode>
  void PUpdate(TNode* node) {
    TBase::PUpdate(node);
    cmax_set = false;
    vmax_set = false;
    if (node->vc) {
      vmax = cmax = node->vc->info.vmax;
      cmax_set = vmax_set = true;
    }
    pmax = node->data;
    if (node->l) {
      pmax = std::max(pmax, node->l->info.pmax);
      if (node->l->info.vmax_set) {
        vmax =
            vmax_set ? std::max(vmax, node->l->info.vmax) : node->l->info.vmax;
        vmax_set = true;
      }
    }
    if (node->r) {
      pmax = std::max(pmax, node->r->info.pmax);
      if (node->r->info.vmax_set) {
        vmax =
            vmax_set ? std::max(vmax, node->r->info.vmax) : node->r->info.vmax;
        vmax_set = true;
      }
    }
    tmax = vmax_set ? std::max(vmax, pmax) : pmax;
  }
};

template <class TMaxType, class TVInfo = VNone>
class VMax : public TVInfo {
 public:
  using TBase = TVInfo;
  using TSelf = VMax<TMaxType, TVInfo>;

  TMaxType vmax;

  template <class TNode>
  void VUpdate(TNode* node) {
    TBase::VUpdate(node);
    vmax = node->vc->info.tmax;
    if (node->l) vmax = std::max(vmax, node->l->info.vmax);
    if (node->r) vmax = std::max(vmax, node->r->info.vmax);
  }
};
}  // namespace info
}  // namespace lcte
}  // namespace graph
