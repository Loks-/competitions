#pragma once

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

  void Merge(const TSelf& r) {
    TBase::Merge(r);
    sum += r.sum;
  }

  template <class TNode>
  void Update(const TNode* node) {
    TBase::Update(node);
    if (node->IsLeaf()) {
      sum = node->ldata;
    } else {
      sum = node->ldata_in_inode ? node->ldata : TSumType();
      if (node->l) sum += node->l->info.sum;
      if (node->r) sum += node->r->info.sum;
    }
  }
};
}  // namespace info
}  // namespace kdtree
}  // namespace geometry
