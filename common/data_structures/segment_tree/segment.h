#pragma once

#include "common/data_structures/segment_tree/info/merge.h"
#include "common/data_structures/segment_tree/info/update_node_to_root.h"

#include <algorithm>
#include <vector>

namespace ds {
namespace st {
template <class TTNode>
class Segment {
 public:
  using TNode = TTNode;
  using TInfo = typename TNode::TInfo;
  using TSelf = Segment<TNode>;

 public:
  std::vector<TNode*> nodes;

 public:
  constexpr Segment() {}

  constexpr explicit Segment(TNode* node) : nodes(1, node) {}

  constexpr Segment(const TSelf& l, const TSelf& r) : nodes(l.nodes) {
    AddBack(r);
  }

  constexpr void AddBack(TNode* node) { nodes.push_back(node); }

  constexpr void AddBack(const TSelf& r) {
    nodes.insert(nodes.end(), r.nodes.begin(), r.nodes.end());
  }

  constexpr void AddBackReversed(const TSelf& r) {
    nodes.insert(nodes.end(), r.nodes.rbegin(), r.nodes.rend());
  }

  constexpr void Reverse() { std::reverse(nodes.begin(), nodes.end()); }

  constexpr TSelf Reversed() const {
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

  template <class TActionValue>
  void AddAction(const TActionValue& value) {
    for (auto node : nodes) {
      node->AddAction(value);
      info::UpdateNodeToRoot(node);
    }
  }
};
}  // namespace st
}  // namespace ds
