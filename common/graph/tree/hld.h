#pragma once

#include "common/graph/tree.h"
#include "common/graph/tree/lca.h"
#include "common/graph/tree/nodes_info.h"
#include "common/segment_tree/action/apply_root_to_node.h"
#include "common/segment_tree/base/get_segment.h"
#include "common/segment_tree/info/update_node_to_root.h"
#include "common/segment_tree/node.h"
#include "common/segment_tree/segment.h"
#include "common/segment_tree/segment_tree.h"

#include <utility>
#include <vector>

namespace graph {
template <class TTData, class TTInfo = st::info::Position<unsigned>,
          class TTAction = st::action::None>
class HLD {
 public:
  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TSTree = st::SegmentTree<TData, TInfo, TAction, true>;
  using TNode = typename TSTree::TNode;
  using TPath = std::vector<TNode*>;
  using TSegment = st::Segment<TNode>;

 protected:
  class Chain {
   public:
    unsigned parent;
    TNode* node;
  };

  class Vertex {
   public:
    unsigned chain;
    TNode* node;
  };

  TSTree stree;
  LCA lca;
  TreeNodesInfo tni;
  std::vector<Chain> chains;
  std::vector<Vertex> vertexes;

 protected:
  void AddNewChain(const TreeGraph& g, unsigned x) {
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
    unsigned chain_index = unsigned(chains.size());
    std::vector<TNode*> nodes;
    for (auto x : cv) {
      vertexes[x].chain = chain_index;
      vertexes[x].node = stree.NewLeaf(TData(), tni.deep[x]);
      nodes.push_back(vertexes[x].node);
    }
    chains.push_back({tni.parent[cv[0]], stree.BuildTree(nodes)});
  }

 public:
  HLD() {}
  HLD(const TreeGraph& tree) { Build(tree); }

  void Build(const TreeGraph& tree) {
    stree.ResetNodes(tree.Size());
    lca.Build(tree);
    tni.Init(tree);
    chains.clear();
    vertexes.resize(tree.Size());
    AddNewChain(tree, tree.GetRoot());
  }

  unsigned Deep(unsigned x) const { return lca.deep[x]; }
  unsigned Chain(unsigned x) const { return vertexes[x].chain; }
  TNode* Node(unsigned x) { return vertexes[x].node; }
  const TNode* Node(unsigned x) const { return vertexes[x].node; }

  unsigned GetLCA(unsigned x, unsigned y) const { return lca.GetLCA(x, y); }

  unsigned DistanceFromAncestor(unsigned a, unsigned x) const {
    return Deep(x) - Deep(a);
  }

  unsigned Distance(unsigned x, unsigned y) const {
    unsigned a = GetLCA(x, y);
    return DistanceFromAncestor(a, x) + DistanceFromAncestor(a, y);
  }

  TSegment PathFromAncestor(unsigned a, unsigned x, bool skip_ancestor) const {
    TSegment s;
    unsigned ac = Chain(a), xc = Chain(x);
    for (; xc != ac; xc = Chain(x)) {
      s.AddBack(st::GetSegment(chains[xc].node, 0, Deep(x)));
      x = chains[xc].parent;
    }
    s.AddBack(st::GetSegment(chains[ac].node, Deep(a) + (skip_ancestor ? 1 : 0),
                             Deep(x)));
    s.Reverse();
    return s;
  }

  TSegment Path(unsigned x, unsigned y) {
    unsigned a = GetLCA(x, y);
    return TSegment(PathFromAncestor(a, x, false).Reversed(),
                    PathFromAncestor(a, y, true));
  }

  void SetData(unsigned x, const TData& data) {
    auto node = Node(x);
    st::action::ApplyRootToNode(node);
    node->GetData() = data;
    st::info::UpdateNodeToRoot(node);
  }
};
}  // namespace graph
