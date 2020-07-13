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
// Doesn't support add and delete operations.
template <bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t>
class PerfectTree
    : public Tree<NodesManagerFixedSize<Node<TData, TInfo, TAction, true,
                                             use_parent, false, TKey>>,
                  PerfectTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  static const bool support_insert = false;
  static const bool support_remove = false;

  using TNode = Node<TData, TInfo, TAction, true, use_parent, false, TKey>;
  using TSelf = PerfectTree<use_parent, TData, TInfo, TAction, TKey>;
  using TTree = Tree<NodesManagerFixedSize<TNode>, TSelf>;
  friend class Tree<NodesManagerFixedSize<TNode>, TSelf>;

 public:
  explicit PerfectTree(size_t max_nodes) : TTree(max_nodes) {}
};
}  // namespace bst
