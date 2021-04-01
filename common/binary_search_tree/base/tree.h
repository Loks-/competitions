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
namespace base {
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

  static const bool support_insert = true;
  static const bool support_remove = use_parent;
  static const bool support_remove_by_node = use_parent && TMe::support_remove;
  static const bool support_join = false;
  static const bool support_split = false;
  static const bool support_insert_by_order =
      !use_key && TInfo::has_size && TMe::support_join && TMe::support_split;

  static const bool is_persistent = false;

 public:
  explicit Tree(size_t max_nodes) : TNodesManager(max_nodes) {}
  Tree() : Tree(0) {}

  TMe* Me() { return static_cast<TMe*>(this); }
  const TMe* Me() const { return static_cast<const TMe*>(this); }

  TNode* New() { return TNodesManager::New(); }

  TNode* New(const TData& data) {
    auto p = New();
    p->data = data;
    p->info.BTIReset();
    p->UpdateInfo();
    return p;
  }

  TNode* New(const TData& data, const TKey& key) {
    static_assert(use_key, "use_key should be true");
    auto p = New();
    p->data = data;
    p->key = key;
    p->info.BTIReset();
    p->UpdateInfo();
    return p;
  }

  static TNode* BuildTree(const std::vector<TNode*>& vnodes) {
    TNode* root = TMe::BuildTreeI(vnodes, 0, vnodes.size());
    if (root) root->SetP(nullptr);
    return root;
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
    return bst::base::FindByKey(root, key);
  }

  static TNode* FindByOrder(TNode* root, size_t order_index) {
    return bst::base::FindByOrder(root, order_index);
  }

  static TNode* InsertByKey(TNode* root, TNode* node) {
    static_assert(TMe::support_insert, "Insert should be supported");
    static_assert(use_key, "use_key should be true");
    assert(node);
    return root ? TMe::InsertByKeyI(root, node) : node;
  }

  static TNode* InsertByOrder(TNode* root, TNode* node, size_t order_index) {
    static_assert(TMe::support_insert_by_order);
    assert(node);
    if (!root) {
      assert(order_index == 0);
      return node;
    } else {
      assert(order_index <= root->info.size);
      TNode *l = nullptr, *r = nullptr;
      TMe::SplitBySize(root, order_index, l, r);
      return TMe::Join3(l, node, r);
    }
  }

  TNode* InsertNewNode(TNode* root, const TData& data, const TKey& key) {
    return TMe::InsertByKey(root, New(data, key));
  }

  TNode* InsertNewNodeByOrder(TNode* root, const TData& data,
                              size_t order_index) {
    return TMe::InsertByOrder(root, New(data), order_index);
  }

  static TNode* RemoveByKey(TNode* root, const TKey& key,
                            TNode*& removed_node) {
    static_assert(TMe::support_remove, "Remove should be supported");
    static_assert(use_key, "use_key should be true");
    return TMe::RemoveByKeyI(root, key, removed_node);
  }

  static TNode* RemoveByOrder(TNode* root, size_t order_index,
                              TNode*& removed_node) {
    static_assert(TMe::support_remove, "Remove should be supported");
    return TMe::RemoveByOrderI(root, order_index, removed_node);
  }

  static TNode* RemoveByNode(TNode* node) {
    static_assert(TMe::support_remove_by_node,
                  "Remove by node should be supported");
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

  static TNode* Join(TNode* l, TNode* r);

  static TNode* Join3(TNode* l, TNode* m1, TNode* r) {
    return TMe::Join(l, TMe::Join(m1, r));
  }

  static void SplitByKey(TNode* root, const TKey& key, TNode*& output_l,
                         TNode*& output_r);
  static void SplitBySize(TNode* root, size_t lsize, TNode*& output_l,
                          TNode*& output_r);

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

  static TNode* InsertByKeyI(TNode* root, TNode* node);

  static TNode* RemoveByKeyI(TNode* root, const TKey& key,
                             TNode*& removed_node) {
    removed_node = TMe::FindByKey(root, key);
    return (removed_node ? TMe::RemoveByNodeI(removed_node) : root);
  }

  static TNode* RemoveByOrderI(TNode* root, size_t order_index,
                               TNode*& removed_node) {
    removed_node = TMe::FindByOrder(root, order_index);
    return (removed_node ? TMe::RemoveByNodeI(removed_node) : root);
  }

  static TNode* RemoveByNodeI(TNode* node);
};
}  // namespace base
}  // namespace bst
