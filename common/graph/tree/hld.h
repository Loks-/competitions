#pragma once

#include "common/base.h"
#include "common/graph/tree.h"
#include "common/graph/tree/nodes_info.h"
#include "common/segment_tree/action/apply_root_to_node.h"
#include "common/segment_tree/base/get_segment.h"
#include "common/segment_tree/info/position.h"
#include "common/segment_tree/info/update_node_to_root.h"
#include "common/segment_tree/node.h"
#include "common/segment_tree/segment.h"
#include "common/segment_tree/segment_tree.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace graph {
using HLDInfo = st::info::Position<uint64_t>;

template <class TTData, class TTInfo = HLDInfo,
          class TTAction = st::action::None>
class HLD {
 public:
  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSTree = st::SegmentTree<TData, TInfo, TAction, true>;
  using TNode = typename TSTree::TNode;
  using TSegment = st::Segment<TNode>;

 protected:
  class Chain {
   public:
    unsigned parent;
    unsigned deep;
    unsigned order;
    TNode* node;
  };

  class Vertex {
   public:
    unsigned chain;
    TNode* node;
  };

  static const uint64_t mask = (uint64_t(1) << 32) - 1;
  TSTree stree;
  TreeNodesInfo tni;
  std::vector<Chain> chains;
  std::vector<Vertex> vertexes;
  TNode* stroot;

 protected:
  void AddNewChain(const TreeGraph& g, unsigned x) {
    unsigned chain_index = unsigned(chains.size());
    chains.push_back({tni.parent[x], tni.deep[x], tni.preorder[x], nullptr});
    uint64_t cp = uint64_t(tni.preorder[x]) << 32;
    std::vector<unsigned> cv;
    for (; x != CNone;) {
      cv.push_back(x);
      unsigned p = tni.parent[x];
      unsigned max_size = 0, max_size_child = CNone;
      for (unsigned y : g.Edges(x)) {
        if (y == p) continue;
        if (max_size < tni.subtree_size[y]) {
          max_size = tni.subtree_size[y];
          max_size_child = y;
        }
      }
      for (unsigned y : g.Edges(x)) {
        if (y == p) continue;
        if (y != max_size_child) AddNewChain(g, y);
      }
      x = max_size_child;
    }
    std::vector<TNode*> nodes;
    for (auto x : cv) {
      vertexes[x].chain = chain_index;
      vertexes[x].node = stree.NewLeaf(TData(), cp + tni.deep[x]);
      nodes.push_back(vertexes[x].node);
    }
    chains[chain_index].node = stree.BuildTree(nodes);
  }

 public:
  HLD() {}
  explicit HLD(const TreeGraph& tree) { Build(tree); }

  void Build(const TreeGraph& tree) {
    stree.ResetNodes(tree.Size());
    tni.Init(tree);
    chains.clear();
    vertexes.resize(tree.Size());
    AddNewChain(tree, tree.GetRoot());
    std::vector<TNode*> v;
    for (auto& c : chains) v.push_back(c.node);
    std::sort(v.begin(), v.end(),
              [](auto& n1, auto& n2) { return n1->info.left < n2->info.left; });
    stroot = stree.BuildTree(v);
  }

  unsigned Deep(unsigned x) const { return tni.deep[x]; }
  unsigned Chain(unsigned x) const { return vertexes[x].chain; }
  TNode* Node(unsigned x) { return vertexes[x].node; }
  const TNode* Node(unsigned x) const { return vertexes[x].node; }

  uint64_t STX(unsigned x) const {
    unsigned c = vertexes[x].chain;
    return (uint64_t(chains[c].order) << 32) + Deep(x);
  }

  unsigned LCA(unsigned x, unsigned y) const {
    for (unsigned xc = vertexes[x].chain, yc = vertexes[y].chain; xc != yc;) {
      if (chains[xc].deep < chains[yc].deep) {
        y = chains[yc].parent;
        yc = vertexes[y].chain;
      } else {
        x = chains[xc].parent;
        xc = vertexes[x].chain;
      }
    }
    return (tni.deep[x] < tni.deep[y]) ? x : y;
  }

  unsigned DistanceFromAncestor(unsigned a, unsigned x) const {
    return Deep(x) - Deep(a);
  }

  unsigned Distance(unsigned x, unsigned y) const {
    unsigned a = LCA(x, y);
    return DistanceFromAncestor(a, x) + DistanceFromAncestor(a, y);
  }

  TSegment PathFromAncestor(unsigned a, unsigned x, bool skip_ancestor) const {
    TSegment s;
    unsigned ac = Chain(a), xc = Chain(x);
    for (; xc != ac; xc = Chain(x)) {
      st::action::ApplyRootToNode(chains[xc].node);
      s.AddBack(st::GetSegment(chains[xc].node, STX(x) & ~mask, STX(x)));
      x = chains[xc].parent;
    }
    st::action::ApplyRootToNode(chains[ac].node);
    s.AddBack(st::GetSegment(chains[ac].node, STX(a) + (skip_ancestor ? 1 : 0),
                             STX(x)));
    s.Reverse();
    return s;
  }

  TSegment Path(unsigned x, unsigned y) {
    unsigned a = LCA(x, y);
    return TSegment(PathFromAncestor(a, x, false).Reversed(),
                    PathFromAncestor(a, y, true));
  }

  void SetData(unsigned x, const TData& data) {
    auto node = Node(x);
    st::action::ApplyRootToNode(node);
    node->GetData() = data;
    st::info::UpdateNodeToRoot(node);
  }

  TSegment Subtree(unsigned x) {
    uint64_t l0 = tni.preorder[x], l1 = l0 + tni.subtree_size[x];
    auto node = chains[vertexes[x].chain].node;
    st::action::ApplyRootToNode(node);
    return TSegment(st::GetSegment(node, STX(x), STX(x) | mask),
                    st::GetSegment(stroot, (l0 + 1) << 32, (l1 << 32) - 1));
  }
};
}  // namespace graph
