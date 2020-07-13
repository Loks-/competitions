#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/apply_root_to_node.h"
#include "common/binary_search_tree/base/find_by_key.h"
#include "common/binary_search_tree/base/find_by_order.h"
#include "common/template.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace bst {
template <class TTNodesManager, class TTMe>
class Tree : public TTNodesManager {
 public:
  using TNodesManager = TTNodesManager;
  using TNode = typename TNodesManager::TNode;
  using TData = typename TNode::TData;
  using TKey = typename TNode::TKey;
  using TInfo = typename TNode::TInfo;
  using TAction = typename TNode::TAction;
  using TMe = TTMe;

  static const bool use_key = TNode::use_key;
  static const bool use_parent = TNode::use_parent;
  static const bool use_height = TNode::use_height;
  static const bool support_insert = true;
  static const bool support_delete = true;
  static const bool support_delete_by_node = use_parent;

 public:
  explicit Tree(size_t max_nodes) : TNodesManager(max_nodes) {}
  Tree() : Tree(0) {}

  TMe* Me() { return static_cast<TMe*>(this); }
  const TMe* Me() const { return static_cast<const TMe*>(this); }

  TNode* New() { return TNodesManager::New(); }

  TNode* New(const TData& data) {
    auto p = New();
    p->data = data;
    p->UpdateInfo();
    return p;
  }

  TNode* New(const TData& data, const TKey& key) {
    static_assert(use_key, "use_key should be true");
    auto p = New();
    p->data = data;
    p->key = key;
    p->UpdateInfo();
    return p;
  }

  static TNode* BuildTree(const std::vector<TNode*>& vnodes) {
    return TMe::BuildTreeI(vnodes, 0, vnodes.size());
  }

  TNode* Build(const std::vector<TData>& data) {
    TNodesManager::ReserveAdditional(data.size());
    if (data.size() == 0) return nullptr;
    std::vector<TNode*> v(data.size());
    for (size_t i = 0; i < data.size(); ++i) v[i] = New(data[i]);
    return TMe::BuildTree(v);
  }

  TNode* Build(const std::vector<TData>& data, const std::vector<TKey>& keys) {
    static_assert(use_key, "use_key should be true");
    assert(data.size() == keys.size());
    TNodesManager::ReserveAdditional(data.size());
    if (data.size() == 0) return nullptr;
    std::vector<std::pair<TKey, TNode*>> vp(data.size());
    for (size_t i = 0; i < data.size(); ++i)
      vp[i] = std::make_pair(keys[i], New(data[i], keys[i]));
    std::sort(vp.begin(), vp.end());
    std::vector<TNode*> v(vp.size());
    for (size_t i = 0; i < vp.size(); ++i) v[i] = vp[i].second;
    return TMe::BuildTree(v);
  }

  static TNode* FindByKey(TNode* root, const TKey& key) {
    return bst::FindByKey(root, key);
  }

  static TNode* FindByOrder(TNode* root, size_t order_index) {
    return bst::FindByOrder(root, order_index);
  }

  static TNode* InsertByKey(TNode* root, TNode* node) {
    static_assert(use_key, "use_key should be true");
    assert(node);
    return root ? TMe::InsertByKeyI(root, node, TFakeBool<use_parent>()) : node;
  }

  TNode* InsertNewNode(TNode* root, const TData& data, const TKey& key) {
    return TMe::InsertByKey(root, New(data, key));
  }

  static TNode* RemoveByKey(TNode* root, const TKey& key,
                            TNode*& removed_node) {
    return TMe::RemoveByKeyI(root, key, removed_node, TFakeBool<use_parent>());
  }

  static TNode* RemoveByOrder(TNode* root, size_t order_index,
                              TNode*& removed_node) {
    return TMe::RemoveByOrderI(root, order_index, removed_node,
                               TFakeBool<use_parent>());
  }

  static TNode* RemoveByNode(TNode* node) {
    static_assert(use_parent, "use_parent should be true");
    assert(node);
    action::ApplyRootToNode(node);
    return TMe::RemoveByNodeI(node);
  }

  TNode* RemoveAndReleaseByNode(TNode* node) {
    TNode* new_root = TMe::RemoveByNode(node);
    TNodesManager::Release(node);
    return new_root;
  }

  TNode* RemoveAndReleaseByKey(TNode* root, const TKey& key) {
    TNode *removed_node = nullptr,
          *new_root = TMe::RemoveByKey(root, key, removed_node);
    if (removed_node) TNodesManager::Release(removed_node);
    return new_root;
  }

  TNode* RemoveAndReleaseByOrder(TNode* root, size_t order_index) {
    TNode *removed_node = nullptr,
          *new_root = TMe::RemoveByOrder(root, order_index, removed_node);
    if (removed_node) TNodesManager::Release(removed_node);
    return new_root;
  }

  void ReleaseTree(TNode* root) {
    if (root) {
      ReleaseTree(root->l);
      ReleaseTree(root->r);
      TNodesManager::Release(root);
    }
  }

 protected:
  static TNode* BuildTreeI(const std::vector<TNode*>& vnodes, size_t first,
                           size_t last) {
    if (first >= last) return nullptr;
    size_t m = (first + last) / 2;
    TNode* root = vnodes[m];
    root->SetL(BuildTreeI(vnodes, first, m));
    root->SetR(BuildTreeI(vnodes, m + 1, last));
    root->UpdateInfo();
    return root;
  }

  static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node,
                             TFakeTrue) {
    removed_node = TMe::FindByKey(root, key);
    return (removed_node ? TMe::RemoveByNodeI(removed_node) : root);
  }

  static TNode* RemoveByOrderI(TNode* root, size_t order_index,
                               TNode*& removed_node, TFakeTrue) {
    removed_node = TMe::FindByOrder(root, order_index);
    return (removed_node ? TMe::RemoveByNodeI(removed_node) : root);
  }
};
}  // namespace bst
