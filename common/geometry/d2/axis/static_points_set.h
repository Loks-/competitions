#pragma once

#include "common/base.h"
#include "common/binary_search_tree/info/segment/get_by_key.h"
#include "common/binary_search_tree/info/segment/size.h"
#include "common/binary_search_tree/persistent/treap.h"
#include "common/geometry/d2/compare/point_xy.h"
#include "common/geometry/d2/point.h"
#include "common/stl/pair.h"
#include "common/template.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace geometry {
namespace d2 {
namespace axis {
// Class for points set with fast CountQ function (O(log N) time).
template <class T>
class StaticPointsSet {
 public:
  using TWeight = unsigned;
  using TPoint = Point<T>;
  using TTree =
      bst::persistent::Treap<true, false, TEmpty, bst::info::segment::Size,
                             bst::action::None, T>;
  using TNode = typename TTree::TNode;
  using TPair = std::pair<T, TNode*>;

 protected:
  TTree tree;
  std::vector<TPair> vx;

 public:
  StaticPointsSet() {}
  StaticPointsSet(const std::vector<TPoint>& points) { Init(points); }

  void Clear() {
    vx.clear();
    tree.ResetNodes();
  }

  void Init(const std::vector<TPoint>& points) {
    Clear();
    auto vp = points;
    std::sort(vp.begin(), vp.end(),
              [](auto& l, auto& r) { return CompareXY(l, r); });
    TNode* root = nullptr;
    for (auto p : vp) {
      root = tree.InsertNewNode(root, TEmpty(), p.y);
      vx.push_back({p.x, root});
    }
  }

  TNode* LowerBound(T x) const {
    auto it = std::lower_bound(vx.begin(), vx.end(), TPair(x, nullptr));
    if (it == vx.begin()) return nullptr;
    return (it - 1)->second;
  }

  TNode* UpperBound(T x) const { return LowerBound(x + 1); }

  unsigned CountQ(const TPoint& p) const {
    return bst::info::segment::GetByKey(LowerBound(p.x), p.y).size;
  }
};
}  // namespace axis
}  // namespace d2
}  // namespace geometry

using I2ASPointsSet = geometry::d2::axis::StaticPointsSet<int64_t>;
