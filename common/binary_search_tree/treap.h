#pragma once
#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/base/root.h"
#include "common/binary_search_tree/base/tree.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/treap_height.h"
#include "common/binary_search_tree/info/update_node_to_root.h"
#include "common/memory/nodes_manager_fixed_size.h"

#include <algorithm>
#include <stack>
#include <vector>

namespace bst {
template <bool use_key, bool use_parent, class TData, class TTInfo = info::Size,
          class TAction = action::None, class TKey = int64_t,
          template <class> class TTNodesManager = memory::NodesManagerFixedSize>
class Treap
    : public base::Tree<
          TTNodesManager<base::Node<TData, info::TreapHeight<unsigned, TTInfo>,
                                    TAction, use_key, use_parent, TKey>>,
          Treap<use_key, use_parent, TData, TTInfo, TAction, TKey,
                TTNodesManager>> {
 public:
  static constexpr bool support_remove = true;
  static constexpr bool support_join = true;
  static constexpr bool support_join3 = true;
  static constexpr bool support_split = true;

  using TInfo = info::TreapHeight<unsigned, TTInfo>;
  using THeight = typename TInfo::THeight;
  using TNode = base::Node<TData, TInfo, TAction, use_key, use_parent, TKey>;
  using TSelf =
      Treap<use_key, use_parent, TData, TTInfo, TAction, TKey, TTNodesManager>;
  using TTree = base::Tree<TTNodesManager<TNode>, TSelf>;
  friend TTree;

 protected:
  static constexpr const THeight& Height(const TNode* node) {
    return node->info.treap_height;
  }

 public:
  explicit Treap(size_t max_nodes) : TTree(max_nodes) {}

 public:
  static TNode* BuildTree(const std::vector<TNode*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    TNode* proot = nodes[0];
    TNode* plast = proot;
    std::stack<TNode*> s;
    for (size_t j = 1; j < nodes.size(); ++j) {
      TNode* pj = nodes[j];
      if (Height(pj) < Height(plast)) {
        plast->SetR(pj);
        s.push(plast);
      } else if (Height(pj) >= Height(proot)) {
        for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
        pj->SetL(proot);
        proot = pj;
      } else {
        for (plast->UpdateInfo(); Height(pj) >= Height(s.top()); s.pop()) {
          plast = s.top();
          plast->UpdateInfo();
        }
        pj->SetL(plast);
        s.top()->SetR(pj);
      }
      plast = pj;
    }
    for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
    return proot;
  }

 protected:
  static TNode* JoinI(TNode* l, TNode* r) {
    if (Height(l) > Height(r)) {
      l->ApplyAction();
      l->SetR(l->r ? JoinI(l->r, r) : r);
      l->UpdateInfo();
      return l;
    } else {
      r->ApplyAction();
      r->SetL(r->l ? JoinI(l, r->l) : l);
      r->UpdateInfo();
      return r;
    }
  }

 public:
  static TNode* Join(TNode* l, TNode* r) {
    return !l ? r : !r ? l : JoinI(l, r);
  }

  static TNode* Join3(TNode* l, TNode* m1, TNode* r) {
    return Join(l, Join(m1, r));
  }

 protected:
  static void SplitByKeyI(TNode* p, const TKey& key, TNode*& output_l,
                          TNode*& output_r) {
    p->ApplyAction();
    if (p->key < key) {
      if (p->r) {
        output_l = p;
        SplitByKeyI(p->r, key, p->r, output_r);
        p->UpdateInfo();
        if (p->r) p->r->SetP(p);
      } else {
        output_l = p;
        output_r = nullptr;
      }
    } else {
      if (p->l) {
        output_r = p;
        SplitByKeyI(p->l, key, output_l, p->l);
        p->UpdateInfo();
        if (p->l) p->l->SetP(p);
      } else {
        output_l = nullptr;
        output_r = p;
      }
    }
  }

 public:
  static void SplitByKey(TNode* root, const TKey& key, TNode*& output_l,
                         TNode*& output_r) {
    static_assert(use_key, "use_key should be true");
    if (!root) {
      output_l = output_r = nullptr;
    } else {
      SplitByKeyI(root, key, output_l, output_r);
      if (output_l) output_l->SetP(nullptr);
      if (output_r) output_r->SetP(nullptr);
    }
  }

 protected:
  static void SplitBySizeI(TNode* p, size_t lsize, TNode*& output_l,
                           TNode*& output_r) {
    p->ApplyAction();
    const size_t hlsize = (p->l ? p->l->info.size : 0);
    if (lsize < hlsize) {
      output_r = p;
      SplitBySizeI(p->l, lsize, output_l, p->l);
      if (p->l) p->l->SetP(p);
    } else if (lsize == hlsize) {
      output_l = p->l;
      output_r = p;
      p->l = nullptr;
    } else if (lsize == hlsize + 1) {
      output_l = p;
      output_r = p->r;
      p->r = nullptr;
    } else {
      output_l = p;
      SplitBySizeI(p->r, lsize - hlsize - 1, p->r, output_r);
      if (p->r) p->r->SetP(p);
    }
    p->UpdateInfo();
  }

 public:
  static void SplitBySize(TNode* root, size_t lsize, TNode*& output_l,
                          TNode*& output_r) {
    static_assert(TInfo::has_size, "info should contain size");
    if (!root) {
      output_l = output_r = nullptr;
    } else if (lsize == 0) {
      output_l = nullptr;
      output_r = root;
    } else if (lsize >= root->info.size) {
      output_l = root;
      output_r = nullptr;
    } else {
      SplitBySizeI(root, lsize, output_l, output_r);
      if (output_l) output_l->SetP(nullptr);
      if (output_r) output_r->SetP(nullptr);
    }
  }

  static TNode* InsertByKey(TNode* root, TNode* node) {
    static_assert(use_key, "use_key should be true");
    if (!root) return node;
    root->ApplyAction();
    if (Height(root) >= Height(node)) {
      if (root->key < node->key)
        root->SetR(InsertByKey(root->r, node));
      else
        root->SetL(InsertByKey(root->l, node));
    } else {
      SplitByKeyI(root, node->key, node->l, node->r);
      if (node->l) node->l->SetP(node);
      if (node->r) node->r->SetP(node);
      root = node;
    }
    root->UpdateInfo();
    return root;
  }

  static TNode* RemoveByKey(TNode* root, const TKey& key,
                            TNode*& removed_node) {
    static_assert(use_key, "use_key should be true");
    if (!root) return root;
    root->ApplyAction();
    if (root->key < key) {
      root->SetR(RemoveByKey(root->r, key, removed_node));
    } else if (root->key > key) {
      root->SetL(RemoveByKey(root->l, key, removed_node));
    } else {
      removed_node = root;
      TNode* l = root->l;
      if (l) l->SetP(nullptr);
      TNode* r = root->r;
      if (r) r->SetP(nullptr);
      root->ResetLinksAndUpdateInfo();
      return Join(l, r);
    }
    root->UpdateInfo();
    return root;
  }

  static TNode* Union(TNode* p1, TNode* p2) {
    if (!p1) return p2;
    if (!p2) return p1;
    if (p1->info.treap_height < p2->info.treap_height) std::swap(p1, p2);
    TNode *pt1 = nullptr, *pt2 = nullptr;
    SplitByKey(p2, p1->key, pt1, pt2);
    p1->SetL(Union(p1->l, pt1));
    p1->SetR(Union(p1->r, pt2));
    p1->UpdateInfo();
    return p1;
  }

 protected:
  static TNode* RemoveByNodeI(TNode* node) {
    TNode* l = node->l;
    if (l) l->SetP(nullptr);
    TNode* r = node->r;
    if (r) r->SetP(nullptr);
    TNode* p = node->p;
    node->ResetLinksAndUpdateInfo();
    TNode* m = Join(l, r);
    if (!p) return m;
    if (node == p->l)
      p->SetL(m);
    else
      p->SetR(m);
    info::UpdateNodeToRoot(p);
    return Root(p);
  }
};
}  // namespace bst
