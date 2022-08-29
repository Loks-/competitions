#pragma once

#include "common/memory/node.h"

namespace graph {
namespace lcte {
template <class TTData, class TTPInfo, class TTVInfo, class TTAction>
class PNode;

template <class TTData, class TTPInfo, class TTVInfo, class TTAction>
class VNode;

template <class TTData, class TTPInfo, class TTVInfo, class TTAction,
          class TTSelf, class TTDual>
class CNode : public memory::Node {
 public:
  using TData = TTData;
  using TPInfo = TTPInfo;
  using TVInfo = TTVInfo;
  using TAction = TTAction;
  using TNodeP = PNode<TData, TPInfo, TVInfo, TAction>;
  using TNodeV = VNode<TData, TPInfo, TVInfo, TAction>;
  using TSelf = TTSelf;
  using TDual = TTDual;

  TSelf *l, *r, *p;
  TDual *vp, *vc;
  TAction action;

  void ResetLinks() {
    l = r = p = nullptr;
    vp = vc = nullptr;
  }

  void SetL(TSelf *new_l) {
    l = new_l;
    if (new_l) new_l->p = static_cast<TSelf *>(this);
  }

  void SetR(TSelf *new_r) {
    r = new_r;
    if (new_r) new_r->p = static_cast<TSelf *>(this);
  }

  void SetVC(TDual *new_vc) {
    vc = new_vc;
    if (new_vc) new_vc->vp = static_cast<TSelf *>(this);
  }

  void SetVP(TDual *new_vp) {
    vp = new_vp;
    if (new_vp) new_vp->vc = static_cast<TSelf *>(this);
  }

  TSelf *GetRL() {
    TSelf *node = static_cast<TSelf *>(this);
    for (node->ApplyAction(); node->l; node->ApplyAction()) node = node->l;
    return node;
  }

  TSelf *GetRR() {
    TSelf *node = static_cast<TSelf *>(this);
    for (node->ApplyAction(); node->r; node->ApplyAction()) node = node->r;
    return node;
  }

  TSelf *GetRP() {
    TSelf *node = static_cast<TSelf *>(this);
    while (node->p) node = node->p;
    return node;
  }

  void ClearAction() { action.Clear(); }

  CNode() { ResetLinks(); }

  void ClearReuse() {
    ResetLinks();
    ClearAction();
  }
};

template <class TTData, class TTPInfo, class TTVInfo, class TTAction>
class PNode : public CNode<TTData, TTPInfo, TTVInfo, TTAction,
                           PNode<TTData, TTPInfo, TTVInfo, TTAction>,
                           VNode<TTData, TTPInfo, TTVInfo, TTAction>> {
 public:
  static const bool is_path = true;
  static const bool is_virtual = false;

  using TBase = CNode<TTData, TTPInfo, TTVInfo, TTAction,
                      PNode<TTData, TTPInfo, TTVInfo, TTAction>,
                      VNode<TTData, TTPInfo, TTVInfo, TTAction>>;

  TTData data;
  TTPInfo info;

  PNode() {}
  explicit PNode(const TTData &_data) : data(_data) {}

  void UpdateInfo() { info.PUpdate(this); }

  template <class TActionValue>
  void AddAction(const TActionValue &value) {
    TBase::action.PAdd(this, value);
  }

  void Reverse() {
    info.Reverse(this);
    TBase::action.Reverse(this);
  }

  void ApplyAction() { TBase::action.PApply(this); }
};

template <class TTData, class TTPInfo, class TTVInfo, class TTAction>
class VNode : public CNode<TTData, TTPInfo, TTVInfo, TTAction,
                           VNode<TTData, TTPInfo, TTVInfo, TTAction>,
                           PNode<TTData, TTPInfo, TTVInfo, TTAction>> {
 public:
  static const bool is_path = false;
  static const bool is_virtual = true;

  using TBase = CNode<TTData, TTPInfo, TTVInfo, TTAction,
                      VNode<TTData, TTPInfo, TTVInfo, TTAction>,
                      PNode<TTData, TTPInfo, TTVInfo, TTAction>>;

  TTVInfo info;

  void UpdateInfo() { info.VUpdate(this); }

  template <class TActionValue>
  void AddAction(const TActionValue &value) {
    TBase::action.VAdd(this, value);
  }

  void ApplyAction() { TBase::action.VApply(this); }
};
}  // namespace lcte
}  // namespace graph
