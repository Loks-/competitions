#pragma once

#include "common/base.h"
#include "common/segment_tree/action/none.h"
#include "common/segment_tree/info/position.h"
#include "common/segment_tree/node.h"
#include "common/vector/enumerate.h"

#include <vector>

namespace st {
template <class TTData, class TTInfo = info::Position<>,
          class TTAction = action::None, bool _use_parent = true>
class SegmentTree {
 public:
  static const bool use_parent = _use_parent;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSelf = SegmentTree<TData, TInfo, TAction, use_parent>;
  using TNode = Node<TData, TInfo, TAction, use_parent>;
  using TCoordinate = typename TInfo::TCoordinate;

 protected:
  std::vector<TData> data;
  std::vector<TNode> nodes;
  unsigned used_data, used_nodes;

 public:
  void ResetNodes(unsigned data_size, unsigned max_nodes) {
    data.resize(0);
    data.resize(data_size);
    nodes.resize(0);
    nodes.resize(max_nodes);
    used_data = used_nodes = 0;
  }

  void ResetNodes(unsigned data_size) { ResetNodes(data_size, 2 * data_size); }
  void ResetNodes() {
    ResetNodes(unsigned(data.size()), unsigned(nodes.size()));
  }

  SegmentTree() { ResetNodes(0); }

  SegmentTree(unsigned data_size) { ResetNodes(data_size); }

  SegmentTree(unsigned data_size, unsigned max_nodes) {
    ResetNodes(data_size, max_nodes);
  }

  unsigned DataUsed() const { return used_data; }
  unsigned UsedNodes() const { return used_nodes; }

  unsigned AvailableNodes() const {
    return unsigned(nodes.size()) - used_nodes;
  }

  TNode* NewNode() {
    assert(used_nodes < nodes.size());
    return &(nodes[used_nodes++]);
  }

  TNode* NewLeaf(const TData& d = TData(),
                 const TCoordinate& x = TCoordinate()) {
    TNode* node = NewNode();
    assert(used_data < data.size());
    node->SetPData(&(data[used_data]));
    data[used_data++] = d;
    node->info.SetCoordinate(x);
    node->UpdateInfo();
    return node;
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
      root->info.SetCoordinate(vx[first]);
    } else {
      unsigned m = (first + last) / 2;
      root->SetL(BuildTreeI(vdata, vx, first, m));
      root->SetR(BuildTreeI(vdata, vx, m + 1, last));
    }
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
    root->UpdateInfo();
    return root;
  }

 public:
  TNode* BuildTree(const std::vector<TData>& vdata) {
    if (vdata.size() == 0) return 0;
    std::vector<TCoordinate> vx =
        Enumerate<TCoordinate>(0, TCoordinate(vdata.size()));
    return BuildTreeI(vdata, vx, 0, unsigned(vdata.size()) - 1);
  }

  TNode* BuildTree(const std::vector<TData>& vdata,
                   const std::vector<TCoordinate>& vx) {
    assert(vdata.size() == vx.size());
    if (vdata.size() == 0) return 0;
    return BuildTreeI(vdata, vx, 0, unsigned(vdata.size()) - 1);
  }

  TNode* BuildTree(const std::vector<TNode*>& vnodes) {
    return vnodes.empty() ? nullptr
                          : BuildTreeI(vnodes, 0, unsigned(vnodes.size()) - 1);
  }
};
}  // namespace st
