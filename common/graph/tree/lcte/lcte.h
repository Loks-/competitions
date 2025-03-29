#pragma once

#include "common/base.h"
#include "common/graph/tree.h"
#include "common/graph/tree/lcte/node.h"
#include "common/memory/contiguous_nodes_manager.h"

#include <stack>
#include <vector>

namespace graph {
namespace lcte {
// Extended Link/Cut Trees with subtree operations
template <class TTData, class TTPInfo, class TTVInfo, class TTAction>
class LCTE {
 public:
  using TData = TTData;
  using TPInfo = TTPInfo;
  using TVInfo = TTVInfo;
  using TAction = TTAction;
  using TPNode = PNode<TData, TPInfo, TVInfo, TAction>;
  using TVNode = VNode<TData, TPInfo, TVInfo, TAction>;
  using TSelf = LCTE<TData, TPInfo, TVInfo, TAction>;

 protected:
  memory::ContiguousNodesManager<TPNode> pmanager;
  memory::ContiguousNodesManager<TVNode> vmanager;

 public:
  template <class TNode>
  static void ApplyActionRootToNode(TNode* node) {
    thread_local std::stack<TNode*> s;
    for (; node; node = node->p) s.push(node);
    for (; !s.empty(); s.pop()) s.top()->ApplyAction();
  }

  template <class TNode>
  static void UpdateNodeToRoot(TNode* node) {
    for (; node; node = node->p) node->UpdateInfo();
  }

 protected:
  TPNode* PNew(const TData& data) {
    auto node = pmanager.create();
    node->data = data;
    return node;
  }

  TVNode* VNew(TPNode* p, TPNode* c) {
    auto node = vmanager.create();
    node->vp = p;
    node->vc = c;
    return node;
  }

  void ReleaseV(TVNode* node) { vmanager.release(node); }

  void DisconnectR(TPNode* node) {
    if (node->r) {
      node->r->p = nullptr;
      auto v = VNew(node, node->r);
      node->r->vp = v;
      node->r = nullptr;
      v->r = node->vc;
      if (v->r) {
        v->r->p = v;
        v->r->vp = nullptr;
      }
      node->vc = v;
      v->UpdateInfo();
      node->UpdateInfo();
    }
  }

  template <class TNode>
  static void Rotate(TNode* c, TNode* p, TNode* gp) {
    if (gp) {
      if (gp->l == p)
        gp->l = c;
      else
        gp->r = c;
    }
    c->p = gp;
    if (p->l == c) {
      p->SetL(c->r);
      c->SetR(p);
    } else {
      p->SetR(c->l);
      c->SetL(p);
    }
    p->UpdateInfo();
  }

  template <class TNode>
  static void RotateUp(TNode* node) {
    if (node && node->p) Rotate(node, node->p, node->p->p);
  }

  // Splay assumes that actions are already applied from root to node.
  template <class TNode>
  static void Splay(TNode* node) {
    // Update vp link first
    auto rp = node->GetRP();
    if (node == rp) return;
    node->SetVP(rp->vp);
    rp->vp = nullptr;
    // Regular Splay
    for (;;) {
      auto p = node->p;
      if (!p) break;
      auto gp = p->p;
      if (!gp) {
        Rotate(node, p, gp);
        break;
      }
      bool zigzig = ((gp->l == p) == (p->l == node));
      RotateUp(zigzig ? p : node);
      RotateUp(node);
    }
    node->UpdateInfo();
  }

 public:
  void Access(TPNode* node) {
    assert(node);
    ApplyActionRootToNode(node);
    Splay(node);
    DisconnectR(node);
    while (node->vp) {
      TVNode* v = node->vp;
      ApplyActionRootToNode(v);
      Splay(v);
      TPNode* p = v->vp;
      assert(p);
      ApplyActionRootToNode(p);
      v->ApplyAction();
      node->ApplyAction();
      Splay(p);
      DisconnectR(p);
      if (p->vc->r == v) {
        assert(!p->vc->l);
        p->vc->SetR(v->r);
        p->vc->SetL(v->l);
        p->vc->UpdateInfo();
      } else {
        assert(p->vc == v);
        if (!v->l) {
          p->SetVC(v->r);
          if (v->r) v->r->p = nullptr;
        } else if (!v->r) {
          p->SetVC(v->l);
          if (v->l) v->l->p = nullptr;
        } else {
          auto r = v->l->GetRR();
          v->l->p = nullptr;
          r->SetR(v->r);
          UpdateNodeToRoot(r);
          p->SetVC(v->l);
        }
      }
      ReleaseV(v);
      node->p = p;
      p->r = node;
      Rotate<TPNode>(node, p, nullptr);
      node->SetVP(p->vp);
      p->vp = nullptr;
    }
    node->UpdateInfo();
  }

  TPNode* FindRoot(TPNode* node) {
    assert(node);
    Access(node);
    node = node->GetRL();
    Splay(node);
    return node;
  }

  void Cut(TPNode* node) {
    Access(node);
    if (node->l) {
      node->l->p = nullptr;
      node->l = nullptr;
      node->UpdateInfo();
    }
  }

  // Link root to node
  void Link(TPNode* root, TPNode* node) {
    Access(root);
    assert(root->l == nullptr);
    Access(node);
    node->p = root;
    root->l = node;
    root->UpdateInfo();
  }

  // Set new root
  void SetRoot(TPNode* node) {
    assert(node);
    Access(node);
    node->Reverse();
  }

  TPNode* TreeParent(TPNode* node) {
    assert(node);
    ApplyActionRootToNode(node);
    return node->l    ? node->l->GetRR()
           : node->vp ? node->vp->GetRP()->vp
                      : nullptr;
  }

  void SelectNode(TPNode* node) {
    assert(node);
    Access(node);
    if (node->l) Splay(node->l->GetRR());
  }

 protected:
  void BuildR(const TreeGraph& tree, unsigned node, unsigned p) {
    auto pnode = Node(node);
    for (unsigned c : tree.Edges(node)) {
      if (c == p) continue;
      auto pc = Node(c);
      auto v = VNew(pnode, pc);
      pc->vp = v;
      v->SetR(pnode->vc);
      if (v->r) v->r->vp = nullptr;
      v->SetVP(pnode);
      BuildR(tree, c, node);
    }
  }

  template <class TNode>
  void UpdateTreeInfo(TNode* node) {
    if (!node) return;
    UpdateTreeInfo(node->l);
    UpdateTreeInfo(node->r);
    UpdateTreeInfo(node->vc);
    node->UpdateInfo();
  }

 public:
  LCTE() {}

  explicit LCTE(unsigned size) { BuildEmpty(std::vector<TData>(size)); }

  explicit LCTE(const std::vector<TData>& data) { BuildEmpty(data); }

  explicit LCTE(const TreeGraph& tree) {
    Build(tree, std::vector<TData>(tree.Size()));
  }

  LCTE(const TreeGraph& tree, const std::vector<TData>& data) {
    Build(tree, data);
  }

  TPNode* Node(unsigned x) { return pmanager.at(x); }
  const TPNode* Node(unsigned x) const { return pmanager.at(x); }

  void BuildEmpty(const std::vector<TData>& data) {
    pmanager.reset(data.size());
    vmanager.reset(data.size());
    for (unsigned i = 0; i < data.size(); ++i) PNew(data[i])->UpdateInfo();
  }

  void Build(const TreeGraph& tree, const std::vector<TData>& data) {
    assert(tree.Size() == data.size());
    pmanager.reset(tree.Size());
    vmanager.reset(tree.Size());
    for (unsigned i = 0; i < tree.Size(); ++i) PNew(data[i]);
    BuildR(tree, tree.GetRoot(), CNone);
    UpdateTreeInfo(Node(tree.GetRoot()));
  }

  void SetData(unsigned x, const TData& data) {
    auto node = Node(x);
    Access(node);
    node->data = data;
    node->UpdateInfo();
  }

  // Reset tree root
  const TPInfo& GetPathInfo(unsigned x, unsigned y) {
    SetRoot(Node(x));
    Access(Node(y));
    return Node(y)->info;
  }

  template <class TActionValue>
  void AddActionToSubtree(unsigned root, unsigned x,
                          const TActionValue& value) {
    SetRoot(Node(root));
    auto node = Node(x);
    SelectNode(node);
    node->AddAction(value);
    assert(!node->vp);
    if (node->p) {
      node->p->UpdateInfo();
    }
  }
};
}  // namespace lcte
}  // namespace graph
