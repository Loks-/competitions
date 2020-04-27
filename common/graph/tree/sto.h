#pragma once

#include "common/graph/tree.h"
#include "common/graph/tree/nodes_info.h"
#include "common/segment_tree/action/apply_root_to_node.h"
#include "common/segment_tree/base/get_segment.h"
#include "common/segment_tree/info/update_node_to_root.h"
#include "common/segment_tree/segment.h"
#include "common/segment_tree/segment_tree.h"

#include <vector>

namespace graph {
// Subtree Operations
template <class TTData, class TTInfo = st::info::None,
          class TTAction = st::action::None,
          class TTSInfo = st::sinfo::Position<unsigned>>
class STO {
  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSInfo = TTSInfo;
  using TSTree = st::SegmentTree<TData, TInfo, TAction, TSInfo, true>;
  using TNode = typename TSTree::TNode;
  using TSegment = st::Segment<TNode>;

 protected:
  TSTree stree;
  TreeNodesInfo tni;
  TNode* stroot;
  std::vector<TNode*> nodes;

 public:
  STO() {}
  explicit STO(const TreeGraph& tree) { Build(tree); }

  void Build(const TreeGraph& tree) {
    unsigned n = tree.Size();
    tni.Init(tree);
    stree.ResetNodes(n);
    nodes.resize(n);
    std::vector<TNode*> nodes_in_order(n, nullptr);
    for (unsigned i = 0; i < n; ++i) {
      unsigned p = tni.preorder[i];
      nodes[i] = stree.NewLeaf(TData(), p);
      nodes_in_order[p] = nodes[i];
    }
    stroot = stree.BuildTree(nodes_in_order);
  }

  unsigned Parent(unsigned x) const { return tni.parent[x]; }
  unsigned Deep(unsigned x) const { return tni.deep[x]; }
  TNode* Node(unsigned x) { return nodes[x]; }
  const TNode* Node(unsigned x) const { return nodes[x]; }

  TSegment Subtree(unsigned x) {
    return st::GetSegment(stroot, tni.preorder[x],
                          tni.preorder[x] + tni.subtree_size[x] - 1);
  }

  const TData& GetData(unsigned x) {
    auto node = Node(x);
    st::action::ApplyRootToNode(node);
    return node->GetData();
  }

  void SetData(unsigned x, const TData& data) {
    auto node = Node(x);
    st::action::ApplyRootToNode(node);
    node->GetData() = data;
    st::info::UpdateNodeToRoot(node);
  }
};
}  // namespace graph
