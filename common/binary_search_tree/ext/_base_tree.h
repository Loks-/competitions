#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/insert_by_key.h"
#include "common/binary_search_tree/base/remove_by_key.h"
#include "common/binary_search_tree/base/remove_by_node.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/node.h"
#include "common/binary_search_tree/tree.h"
#include "common/memory/nodes_manager_fixed_size.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace bst {
// Base BST without auto balance
template <bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t>
class BaseTree
    : public Tree<NodesManagerFixedSize<Node<TData, TInfo, TAction, true,
                                             use_parent, false, TKey>>,
                  BaseTree<use_parent, TData, TInfo, TAction, TKey>> {
 public:
  using TNode = Node<TData, TInfo, TAction, true, use_parent, false, TKey>;
  using TSelf = BaseTree<use_parent, TData, TInfo, TAction, TKey>;
  using TTree = Tree<NodesManagerFixedSize<TNode>, TSelf>;
  friend class Tree<NodesManagerFixedSize<TNode>, TSelf>;

 public:
  explicit BaseTree(size_t max_nodes) : TTree(max_nodes) {}

  static TNode* InsertByKey(TNode* root, TNode* node) {
    assert(node);
    if (!root) return node;
    bst::InsertByKey(root, node);
    return root;
  }

  static TNode* RemoveByKey(TNode* root, const TKey& key,
                            TNode*& removed_node) {
    return bst::RemoveByKey(root, key, removed_node);
  }

 protected:
  static TNode* RemoveByNodeI(TNode* node) { return bst::RemoveByNodeI(node); }
};
}  // namespace bst
