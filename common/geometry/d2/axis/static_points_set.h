#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/segment/get_by_key.h"
#include "common/binary_search_tree/info/segment/none.h"
#include "common/binary_search_tree/info/segment/sum.h"
#include "common/binary_search_tree/persistent/treap.h"
#include "common/geometry/d2/compare/point_xy.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/wpoint.h"
#include "common/stl/pair.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace geometry {
namespace d2 {
namespace axis {
template <class T1, class T2 = unsigned>
class StaticPointsSet {
 public:
  using TPoint = Point<T1>;
  using TWPoint = WPoint<T1, T2>;
  using TTree = bst::persistent::Treap<
      true, false, T2, bst::info::segment::Sum<T2, bst::info::segment::None>,
      bst::action::None, T1>;
  using TNode = typename TTree::TNode;
  using TPair = std::pair<T1, TNode*>;

 protected:
  TTree tree;
  std::vector<TPair> vx;

 public:
  StaticPointsSet() {}
  StaticPointsSet(const std::vector<TPoint>& points) { Init(points); }
  StaticPointsSet(const std::vector<TWPoint>& points) { Init(points); }

  void Clear() {
    vx.clear();
    tree.ResetNodes();
  }

  void Init(const std::vector<TPoint>& points) {
    std::vector<TWPoint> v;
    for (auto p : points) v.push_back(p);
    Init(v);
  }

  void Init(const std::vector<TWPoint>& points) {
    Clear();
    auto vp = points;
    std::sort(vp.begin(), vp.end(),
              [](auto& l, auto& r) { return CompareXY(l, r); });
    TNode* root = nullptr;
    for (auto p : vp) {
      root = tree.InsertNewNode(root, p.w, p.y);
      vx.push_back({p.x, root});
    }
  }

  TNode* LowerBound(T1 x) const {
    auto it = std::lower_bound(vx.begin(), vx.end(), TPair(x, nullptr));
    if (it == vx.begin()) return nullptr;
    return (it - 1)->second;
  }

  TNode* UpperBound(T1 x) const { return LowerBound(x + 1); }

  T2 CountQ(const TPoint& p) const {
    return bst::info::segment::GetByKey(LowerBound(p.x), p.y).sum;
  }
};
}  // namespace axis
}  // namespace d2
}  // namespace geometry

using I2ASPointsSet = geometry::d2::axis::StaticPointsSet<int64_t>;
