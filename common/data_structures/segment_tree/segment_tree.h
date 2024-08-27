#pragma once

#include "common/base.h"
#include "common/data_structures/segment_tree/action/none.h"
#include "common/data_structures/segment_tree/info/none.h"
#include "common/data_structures/segment_tree/node.h"
#include "common/data_structures/segment_tree/sinfo/position.h"
#include "common/vector/enumerate.h"

#include <vector>

namespace ds {
namespace st {
template <class TTData, class TTInfo = info::None,
          class TTAction = action::None, class TTSInfo = sinfo::Position<>,
          bool _use_parent = true>
class SegmentTree {
 public:
  static constexpr bool use_parent = _use_parent;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSInfo = TTSInfo;
  using TSelf = SegmentTree<TData, TInfo, TAction, TSInfo, use_parent>;
  using TNode = Node<TData, TInfo, TAction, TSInfo, use_parent>;
  using TCoordinate = typename TNode::TCoordinate;

 protected:
  std::vector<TData> data;
  std::vector<TNode> nodes;
  unsigned used_data, used_nodes;

 public:
  constexpr void ResetNodes(unsigned data_size, unsigned max_nodes) {
    data.resize(0);
    data.resize(data_size);
    nodes.resize(0);
    nodes.resize(max_nodes);
    used_data = used_nodes = 0;
  }

  constexpr void ResetNodes(unsigned data_size) {
    ResetNodes(data_size, 2 * data_size);
  }

  constexpr void ResetNodes() {
    ResetNodes(unsigned(data.size()), unsigned(nodes.size()));
  }

  constexpr explicit SegmentTree(unsigned data_size) { ResetNodes(data_size); }

  constexpr SegmentTree() : SegmentTree(0) {}

  constexpr SegmentTree(unsigned data_size, unsigned max_nodes) {
    ResetNodes(data_size, max_nodes);
  }

  constexpr unsigned DataUsed() const { return used_data; }

  constexpr unsigned UsedNodes() const { return used_nodes; }

  constexpr unsigned AvailableNodes() const {
    return unsigned(nodes.size()) - used_nodes;
  }

  constexpr TNode* NewNode() {
    assert(used_nodes < nodes.size());
    return &(nodes[used_nodes++]);
  }

  TNode* NewLeaf(const TData& d, const TCoordinate& l, const TCoordinate& r) {
    TNode* node = NewNode();
    assert(used_data < data.size());
    node->SetPData(&(data[used_data]));
    data[used_data++] = d;
    node->sinfo.SetCoordinate(l, r);
    node->UpdateSInfo();
    node->UpdateInfo();
    return node;
  }

  TNode* NewLeaf(const TData& d = TData()) {
    static_assert(TNode::TSInfo::has_coordinate,
                  "has_coordinate should be false");
    return NewLeaf(d, {}, {});
  }

 protected:
  TNode* BuildTreeI(const std::vector<TData>& vdata,
                    const std::vector<TCoordinate>& vx, unsigned first,
                    unsigned last) {
    TNode* root = NewNode();
    if (first == last) {
      assert(used_data < data.size());
      root->SetPData(&(data[used_data]));
      data[used_data++] = vdata[first];
      root->sinfo.SetCoordinate(vx[first], vx[first + 1]);
    } else {
      unsigned m = (first + last) / 2;
      root->SetL(BuildTreeI(vdata, vx, first, m));
      root->SetR(BuildTreeI(vdata, vx, m + 1, last));
    }
    root->UpdateSInfo();
    root->UpdateInfo();
    return root;
  }

  TNode* BuildTreeI(const std::vector<TNode*>& vnodes, unsigned first,
                    unsigned last) {
    if (first == last) return vnodes[first];
    TNode* root = NewNode();
    unsigned m = (first + last) / 2;
    root->SetL(BuildTreeI(vnodes, first, m));
    root->SetR(BuildTreeI(vnodes, m + 1, last));
    root->UpdateSInfo();
    root->UpdateInfo();
    return root;
  }

 public:
  TNode* BuildTree(const std::vector<TData>& vdata) {
    if (vdata.size() == 0) return nullptr;
    std::vector<TCoordinate> vx =
        nvector::Enumerate<TCoordinate>(0, TCoordinate(vdata.size() + 1));
    return BuildTreeI(vdata, vx, 0, unsigned(vdata.size()) - 1);
  }

  TNode* BuildTree(const std::vector<TData>& vdata,
                   const std::vector<TCoordinate>& vx) {
    assert(vdata.size() + 1 == vx.size());
    if (vdata.size() == 0) return nullptr;
    return BuildTreeI(vdata, vx, 0, unsigned(vdata.size()) - 1);
  }

  TNode* BuildTree(const std::vector<TNode*>& vnodes) {
    return vnodes.empty() ? nullptr
                          : BuildTreeI(vnodes, 0, unsigned(vnodes.size()) - 1);
  }
};
}  // namespace st
}  // namespace ds
