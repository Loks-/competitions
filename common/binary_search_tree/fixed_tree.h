#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/node.h"
#include "common/binary_search_tree/tree.h"
#include "common/nodes_manager_fixed_size.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace bst {
template <bool _use_parent, class TTData, class TTInfo = info::Size,
          class TTAction = action::None, class TTKey = int64_t>
class FixedTree
    : protected Tree<NodesManagerFixedSize<Node<TTData, TTInfo, TTAction, true,
                                                _use_parent, false, TTKey>>,
                     FixedTree<_use_parent, TTData, TTInfo, TTAction, TTKey>> {
 public:
  static const bool use_key = true;
  static const bool use_parent = _use_parent;
  static const bool use_height = false;

  using TData = TTData;
  using TInfo = TTInfo;
  using TAction = TTAction;
  using TKey = TTKey;
  using TNode =
      Node<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
  using TSelf = FixedTree<use_parent, TData, TTInfo, TAction, TKey>;
  using TTree = Tree<NodesManagerFixedSize<TNode>, TSelf>;
  friend class Tree<NodesManagerFixedSize<TNode>, TSelf>;

 public:
  FixedTree(size_t max_nodes) : TTree(max_nodes) {}

  TNode* Build(const std::vector<TData>& data) { return TTree::Build(data); }

  TNode* Build(const std::vector<TData>& data, const std::vector<TKey>& keys) {
    return TTree::Build(data, keys);
  }
};
}  // namespace bst
