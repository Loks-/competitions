#pragma once

#include "common/base.h"
#include "common/binary_search_tree/base/deferred.h"
#include "common/binary_search_tree/base/extended_tree.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/memory/contiguous_nodes_manager.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace bst {
// Doesn't support add and delete operations.
template <bool use_parent, class TData,
          class TAggregatorsTuple = std::tuple<subtree_data::Size>,
          class TDeferredTuple = std::tuple<>, class TKey = int64_t>
class PerfectTree
    : public base::ExtendedTree<
          memory::ContiguousNodesManager<base::Node<
              TData, base::SubtreeData<TAggregatorsTuple>,
              base::Deferred<TDeferredTuple>, use_parent, true, TKey>>,
          PerfectTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple,
                      TKey>> {
 public:
  static constexpr bool support_insert = false;
  static constexpr bool support_remove = false;
  static constexpr bool support_join3 = false;
  static constexpr bool support_split = false;

  using TSubtreeData = base::SubtreeData<TAggregatorsTuple>;
  using TDeferred = base::Deferred<TDeferredTuple>;
  using TNode =
      base::Node<TData, TSubtreeData, TDeferred, use_parent, true, TKey>;
  using TSelf =
      PerfectTree<use_parent, TData, TAggregatorsTuple, TDeferredTuple, TKey>;
  using TTree =
      base::ExtendedTree<memory::ContiguousNodesManager<TNode>, TSelf>;
  friend TTree;

 public:
  explicit PerfectTree(size_t max_nodes) : TTree(max_nodes) {}
};
}  // namespace bst
