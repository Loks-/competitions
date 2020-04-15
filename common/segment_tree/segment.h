#pragma once

#include "common/segment_tree/info/merge.h"

#include <algorithm>
#include <vector>

namespace st {
template <class TTNode>
class Segment {
 public:
  using TNode = TTNode;
  using TInfo = typename TNode::TInfo;
  using TSelf = Segment<TNode>;

  std::vector<TNode*> nodes;

  Segment() {}
  Segment(TNode* node) : nodes(1, node) {}
  Segment(const TSelf& l, const TSelf& r) : nodes(l.nodes) { AddBack(r); }

  void AddBack(TNode* node) { nodes.push_back(node); }
  void AddBack(const TSelf& r) {
    nodes.insert(nodes.end(), r.nodes.begin(), r.nodes.end());
  }

  void Reverse() { std::reverse(nodes.begin(), nodes.end()); }
  TSelf Reversed() const {
    TSelf t(*this);
    t.Reverse();
    return t;
  }

  TInfo GetInfo() const {
    if (nodes.empty()) return TInfo();
    TInfo t(nodes[0]->info);
    for (size_t i = 1; i < nodes.size(); ++i)
      t = info::MergeLR(t, nodes[i]->info);
    return t;
  }
};
}  // namespace st
