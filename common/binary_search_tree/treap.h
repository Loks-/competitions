#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/update_node_to_root.h"
#include "common/binary_search_tree/node.h"
#include "common/binary_search_tree/tree.h"
#include "common/nodes_manager_fixed_size.h"

#include <stack>
#include <vector>

namespace bst {
template <bool use_key, bool use_parent, class TData, class TInfo = info::Size,
          class TAction = action::None, class TKey = int64_t,
          template <class> class TTNodesManager = NodesManagerFixedSize>
class Treap
    : public Tree<TTNodesManager<Node<TData, TInfo, TAction, use_key,
                                      use_parent, true, TKey, unsigned>>,
                  Treap<use_key, use_parent, TData, TInfo, TAction, TKey,
                        TTNodesManager>> {
 public:
  using TNode =
      Node<TData, TInfo, TAction, use_key, use_parent, true, TKey, unsigned>;
  using TSelf =
      Treap<use_key, use_parent, TData, TInfo, TAction, TKey, TTNodesManager>;
  using TTree = Tree<TTNodesManager<TNode>, TSelf>;
  friend class Tree<TTNodesManager<TNode>, TSelf>;

 public:
  explicit Treap(size_t max_nodes) : TTree(max_nodes) {}

 public:
  static TNode* BuildTree(const std::vector<TNode*>& nodes) {
    if (nodes.size() == 0) return nullptr;
    TNode* proot = nodes[0];
    TNode* plast = proot;
    std::stack<TNode*> s;
    s.push(proot);
    for (size_t j = 1; j < nodes.size(); ++j) {
      TNode* pj = nodes[j];
      if (pj->height < plast->height) {
        plast->SetR(pj);
        s.push(plast);
      } else if (pj->height >= proot->height) {
        for (plast->UpdateInfo(); !s.empty(); s.pop()) s.top()->UpdateInfo();
        pj->SetL(proot);
        proot = pj;
        s.push(proot);
      } else {
        for (plast->UpdateInfo(); pj->height >= s.top()->height; s.pop()) {
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
    if (l->height > r->height) {
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
    size_t hlsize = (p->l ? p->l->info.size : 0);
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
    if (root->height >= node->height) {
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
