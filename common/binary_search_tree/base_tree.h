#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/balanced_tree.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/nodes_manager_fixed_size.h"

namespace bst {
// Base BST without auto balance
template <bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t>
class BaseTree
    : public base::BalancedTree<
          memory::NodesManagerFixedSize<base::Node<
              TData, base::SubtreeData<TAggregatorsTuple>,
              base::Deferred<TDeferredTuple>, true, use_parent, TKey>>,
          BaseTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple,
                   TKey>> {
 public:
  using TSubtreeData = base::SubtreeData<TAggregatorsTuple>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode =
      base::Node<TData, TSubtreeData, TDeferred, true, use_parent, TKey>;
  using TSelf =
      BaseTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple, TKey>;
  using TBTree =
      base::BalancedTree<memory::NodesManagerFixedSize<TNode>, TSelf>;
  friend TBTree;

  static constexpr bool support_join3 = true;

 public:
  explicit BaseTree(size_t max_nodes) : TBTree(max_nodes) {}

 protected:
  static TNode* FixBalance(TNode* node) { return node; }

 public:
  static TNode* Join3(TNode* l, TNode* m1, TNode* r) {
    return TBTree::Join3IBase(l, m1, r);
  }
};
}  // namespace bst
