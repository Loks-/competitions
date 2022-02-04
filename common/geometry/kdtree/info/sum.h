#pragma once

#include "common/geometry/kdtree/base/volume.h"

namespace geometry {
namespace kdtree {
namespace info {
template <class TSumType, class TInfo>
class Sum : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = Sum<TSumType, TInfo>;

  static const bool is_none = false;
  static const bool use_data = true;

  TSumType sum;

  Sum() : sum() {}

  void Merge(const TSelf& r) {
    TBase::Merge(r);
    sum += r.sum;
  }

  template <class TNode>
  void Update(const TNode* node) {
    assert(!node->IsLeaf());
    static_assert(!TNode::ldata_in_inode);
    TBase::Update(node);
    sum = TSumType();
    if (node->l) sum += node->l->info.sum;
    if (node->r) sum += node->r->info.sum;
  }

  template <class TNode, class TPoint>
  void UpdateLeaf(const TNode* node, const TPoint& pb, const TPoint& pe) {
    TBase::UpdateLeaf(node, pb, pe);
    sum = node->ldata * base::Volume(pb, pe);
  }
};
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
