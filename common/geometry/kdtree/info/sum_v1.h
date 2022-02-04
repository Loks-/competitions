#pragma once

namespace geometry {
namespace kdtree {
namespace info {
// Assume that all leafs with non-zero value have volume 1.
template <class TSumType, class TInfo>
class SumV1 : public TInfo {
 public:
  using TBase = TInfo;
  using TSelf = SumV1<TSumType, TInfo>;

  static const bool is_none = false;
  static const bool use_data = true;

  TSumType sum_v1;

  SumV1() : sum_v1() {}

  void Merge(const TSelf& r) {
    TBase::Merge(r);
    sum_v1 += r.sum_v1;
  }

  template <class TNode>
  void Update(const TNode* node) {
    TBase::Update(node);
    if (node->IsLeaf()) {
      sum_v1 = node->ldata;
    } else {
      sum_v1 = node->ldata_in_inode ? node->ldata : TSumType();
      if (node->l) sum_v1 += node->l->info.sum_v1;
      if (node->r) sum_v1 += node->r->info.sum_v1;
    }
  }
};
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
