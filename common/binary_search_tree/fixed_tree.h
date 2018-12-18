#pragma once

#include "action/none.h"
#include "info/size.h"
#include "node.h"
#include "nodes_manager.h"
#include "../base.h"
#include <algorithm>
#include <utility>
#include <vector>

template <bool _use_parent, class TTData, class TTInfo = BSTInfoSize,
          class TTAction = BSTActionNone, class TTKey = int64_t>
class FixedTree
    : public BSTNodesManager<
          BSTNode<TTData, TTInfo, TTAction, true, _use_parent, false, TTKey>> {
 public:
  static const bool use_key = true;
  static const bool use_parent = _use_parent;
  static const bool use_height = false;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TKey = TTKey;
  using TNode =
      BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
  using TSelf = FixedTree<use_parent, TData, TTInfo, TAction, TKey>;
  using TNodesManager = BSTNodesManager<TNode>;

 public:
  FixedTree(unsigned max_nodes) : TNodesManager(max_nodes) {}

  static TNode* BuildTreeI(const std::vector<TNode*>& vnodes, unsigned first,
                           unsigned last) {
    if (first >= last) return 0;
    unsigned m = (first + last) / 2;
    TNode* root = vnodes[m];
    root->SetL(BuildTreeI(vnodes, first, m));
    root->SetR(BuildTreeI(vnodes, m + 1, last));
    root->UpdateInfo();
    return root;
  }

  static TNode* BuildTree(const std::vector<TNode*>& vnodes) {
    return BuildTreeI(vnodes, 0, unsigned(vnodes.size()));
  }

  TNode* Build(const std::vector<TData>& data, const std::vector<TKey>& keys) {
    assert(data.size() == keys.size());
    assert(TNodesManager::AvailableNodes() >= data.size());
    if (data.size() == 0) return 0;
    std::vector<std::pair<TKey, TNode*>> vp(data.size());
    for (unsigned i = 0; i < data.size(); ++i) {
      vp[i] =
          std::make_pair(keys[i], TNodesManager::GetNewNode(data[i], keys[i]));
    }
    std::sort(vp.begin(), vp.end());
    std::vector<TNode*> v(vp.size());
    for (unsigned i = 0; i < vp.size(); ++i) v[i] = vp[i].second;
    return BuildTree(v);
  }
};
