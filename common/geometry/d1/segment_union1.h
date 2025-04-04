#pragma once

#include "common/base.h"
#include "common/data_structures/segment_tree/action/add_each__lsum0.h"
#include "common/data_structures/segment_tree/base/add_action_to_segment.h"
#include "common/data_structures/segment_tree/info/lsum1.h"
#include "common/data_structures/segment_tree/segment_tree.h"
#include "common/data_structures/segment_tree/sinfo/position.h"
#include "common/template.h"

#include <vector>

namespace geometry {
namespace d1 {
template <class TValue>
class SegmentUnion1 {
 public:
  using TTree = ds::st::SegmentTree<MetaEmpty, ds::st::info::LSum1<int, TValue>,
                                    ds::st::action::AddEachLSum0<int>,
                                    ds::st::sinfo::Position<TValue>, false>;
  using TNode = typename TTree::TNode;

 protected:
  TTree tree;
  TNode* root;
  TValue l;

 public:
  SegmentUnion1() { Clear(); }

  void Clear() {
    root = nullptr;
    l = TValue();
  }

  // Sorted list of possible values
  void Init(const std::vector<TValue>& v) {
    assert(!v.empty());
    Clear();
    tree.ResetNodes(v.size());
    root = tree.BuildTree(std::vector<MetaEmpty>(v.size() - 1), v);
    l = v.back() - v[0];
  }

  TValue LengthAll() const { return l; }
  TValue Length0() const { return root->info.lsum0; }
  TValue Length1() const { return root->info.lsum1; }
  TValue Length2Plus() const { return l - root->info.lsum0 - root->info.lsum1; }

  void Adjust(const TValue& l, const TValue& r, int count) {
    ds::st::AddActionToSegment(root, l, r, count);
  }

  void Add(const TValue& l, const TValue& r) { Adjust(l, r, 1); }
  void Remove(const TValue& l, const TValue& r) { Adjust(l, r, -1); }
};
}  // namespace d1
}  // namespace geometry
