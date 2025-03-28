#pragma once

#include "common/binary_search_tree/base/tree.h"

namespace bst {
namespace persistent {
template <class TTNodesManager, class TTMe>
class Tree : public bst::base::Tree<TTNodesManager, TTMe> {
 public:
  using TNodesManager = TTNodesManager;
  using TNode = typename TNodesManager::TNode;
  using TData = typename TNode::DataType;
  using TKey = typename TNode::KeyType;
  using TBase = bst::base::Tree<TTNodesManager, TTMe>;
  using TMe = TTMe;

  static constexpr bool is_persistent = true;

 public:
  Tree() : TBase() {}
  explicit Tree(size_t expected_nodes) : TBase(expected_nodes) {}

  TNode* InsertNewNode(TNode* root, const TData& data, const TKey& key) {
    return TBase::Me()->InsertByKey(root, TBase::New(data, key));
  }

  TNode* PClone(TNode* node) {
    TNode* new_node = TBase::New();
    *new_node = *node;
    return new_node;
  }

  static void UpdatePForChildren(TNode* node) {
    if (node) {
      if (node->left) node->left->set_parent(node);
      if (node->right) node->right->set_parent(node);
    }
  }
};
}  // namespace persistent
}  // namespace bst
