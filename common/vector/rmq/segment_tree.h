#pragma once

#include "common/base.h"
#include "common/data_structures/segment_tree/action/none.h"
#include "common/data_structures/segment_tree/base/get_segment_info.h"
#include "common/data_structures/segment_tree/info/min_with_index.h"
#include "common/data_structures/segment_tree/info/none.h"
#include "common/data_structures/segment_tree/segment_tree.h"
#include "common/data_structures/segment_tree/sinfo/position.h"
#include "common/vector/rmq/position_value.h"

#include <vector>

namespace nvector {
namespace rmq {
// O(N) memory, O(N) preprocessing time, O(log N) request time
template <class TTValue>
class SegmentTree {
 public:
  using TValue = TTValue;
  using TPositionValue = PositionValue<TValue>;
  using TSTree = ds::st::SegmentTree<
      TValue, ds::st::info::MinWithIndex<TValue, ds::st::info::None>,
      ds::st::action::None, ds::st::sinfo::Position<>, false>;
  using TNode = typename TSTree::TNode;

 protected:
  TSTree tree;
  TNode* root = nullptr;

 public:
  SegmentTree() {}
  SegmentTree(const std::vector<TValue>& v) { Build(v); }

  void Build(const std::vector<TValue>& v) {
    tree.ResetNodes(v.size());
    root = tree.BuildTree(v);
  }

  TPositionValue Minimum(size_t b, size_t e) const {
    assert(b < e);
    auto s = ds::st::GetSegmentInfo(root, b, e);
    return {s.min_coordinate, s.min};
  }
};
}  // namespace rmq
}  // namespace nvector
