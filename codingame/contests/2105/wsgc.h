#pragma once

#include "action.h"
#include "position.h"

#include "common/base.h"
#include "common/template.h"

#include <algorithm>
#include <array>
#include <vector>

template <class TNodeInfo>
class WSGC {
 public:
  class Node {
   public:
    TNodeInfo s;
    Action action;

    template <class T>
    void Update(const Action& a, const T& x) {
      assert(action == a);
      FakeUse(a);
      s.Update(x);
    }
  };

  class Leaf {
   public:
    TNodeInfo s;
    std::vector<Node> nodes;
    unsigned k = 0;

    bool Empty() const { return nodes.empty(); }

    bool Has(const Action& a) const {
      auto it = std::lower_bound(nodes.begin(), nodes.end(), a,
                                 [](auto& l, auto& r) { return l.action < r; });
      return !(it == nodes.end()) && (it->action == a);
    }

    template <class T>
    void Update(const Action& a, const T& x) {
      auto it = std::lower_bound(nodes.begin(), nodes.end(), a,
                                 [](auto& l, auto& r) { return l.action < r; });
      it->Update(a, x);
      s.Update(x);
    }

    template <class TActionIterator>
    void InitS(TActionIterator begin, TActionIterator end) {
      size_t n = end - begin;
      nodes.resize(n);
      for (size_t i = 0; i < n; ++i) nodes[i].action = *(begin + i);
    }

    Action GetAction() { return nodes[(k++) % nodes.size()].action; }

    template <class TCompare>
    Action GetBestAction(TCompare cmp) const {
      unsigned b = 0;
      for (unsigned i = 1; i < nodes.size(); ++i) {
        if (cmp(nodes[i].s, nodes[b].s)) b = i;
      }
      return nodes[b].action;
    }
  };

  class Wait : public Node {
   public:
    bool Empty() const { return false; }

    template <class T>
    void Update(const Action& a, const T& x) {
      assert(a.IsWait());
      FakeUse(a);
      Node::s.Update(x);
    }

    template <class TActionIterator>
    void InitS(const Position&, TActionIterator begin, TActionIterator end) {
      assert(end - begin == 1);
      assert(begin->IsWait());
      FakeUse(end);
      Node::action = *begin;
    }

    Action GetAction() { return Node::action; }

    template <class TCompare>
    Action GetBestAction(TCompare) const {
      return Node::action;
    }
  };

  class Seed {
   public:
    TNodeInfo s;
    std::vector<Leaf> leafs;
    unsigned k = 0;

    bool Empty() const { return leafs.empty(); }

    template <class T>
    void Update(const Action& a, const T& x) {
      auto it = std::lower_bound(
          leafs.begin(), leafs.end(), a.value2,
          [](auto& l, auto& r) { return l.nodes[0].action.value2 < r; });
      assert(it != leafs.end());
      it->Update(a, x);
      s.Update(x);
    }

    template <class TActionIterator>
    void InitS(const Position&, TActionIterator begin, TActionIterator end) {
      leafs.clear();
      for (auto it = begin; begin != end; begin = it) {
        for (; (it < end) && (it->value2 == begin->value2);) ++it;
        leafs.push_back({});
        leafs.back().InitS(begin, it);
      }
    }

    Action GetAction() { return leafs[(k++) % leafs.size()].GetAction(); }

    template <class TCompare>
    Action GetBestAction(TCompare cmp) const {
      unsigned b = 0;
      for (unsigned i = 1; i < leafs.size(); ++i) {
        if (cmp(leafs[i].s, leafs[b].s)) b = i;
      }
      return leafs[b].GetBestAction(cmp);
    }
  };

  class Grow {
   public:
    TNodeInfo s;
    std::array<Leaf, 3> leafs;
    bool empty;
    unsigned k = 2;

    bool Empty() const { return empty; }

    template <class T>
    void Update(const Action& a, const T& x) {
      bool f = false;
      for (auto& l : leafs) {
        if (l.Has(a)) {
          f = true;
          l.Update(a, x);
          break;
        }
      }
      assert(f);
      FakeUse(f);
      s.Update(x);
    }

    template <class TActionIterator>
    void InitS(const Position& p, TActionIterator begin, TActionIterator end) {
      thread_local std::array<std::vector<Action>, 3> t;
      for (auto& it : t) it.clear();
      empty = (begin == end);
      for (auto it = begin; it < end; ++it) {
        Action a = *it;
        unsigned ts = p.GetTreeByCell(a.value1).size;
        assert(ts < 3);
        t[ts].push_back(a);
      }
      for (unsigned i = 0; i < 3; ++i) leafs[i].InitS(t[i].begin(), t[i].end());
    }

    Action GetAction() {
      for (++k; leafs[k % 3].Empty();) ++k;
      return leafs[k % 3].GetAction();
    }

    template <class TCompare>
    Action GetBestAction(TCompare cmp) const {
      unsigned b = 0;
      for (; leafs[b].Empty();) ++b;
      for (unsigned i = b + 1; i < leafs.size(); ++i) {
        if (!leafs[i].Empty() && cmp(leafs[i].s, leafs[b].s)) b = i;
      }
      return leafs[b].GetBestAction(cmp);
    }
  };

  class Complete {
   public:
    TNodeInfo s;
    std::array<Leaf, 3> leafs;
    bool empty;
    unsigned k = 2;

    bool Empty() const { return empty; }

    template <class T>
    void Update(const Action& a, const T& x) {
      unsigned ri = (a.value1 < 7)    ? 0
                    : (a.value1 < 19) ? 1
                    : (a.value1 < 37) ? 2
                                      : 3;
      assert(ri < 3);
      leafs[ri].Update(a, x);
      s.Update(x);
    }

    template <class TActionIterator>
    void InitS(const Position&, TActionIterator begin, TActionIterator end) {
      auto F = [&](unsigned x) {
        return std::lower_bound(begin, end, x,
                                [](auto& l, auto& r) { return l.value1 < r; });
      };
      empty = (begin == end);
      auto it0 = begin, it1 = F(7), it2 = F(19), it3 = end;
      leafs[0].InitS(it0, it1);
      leafs[1].InitS(it1, it2);
      leafs[2].InitS(it2, it3);
    }

    Action GetAction() {
      for (++k; leafs[k % 3].Empty();) ++k;
      return leafs[k % 3].GetAction();
    }

    template <class TCompare>
    Action GetBestAction(TCompare cmp) const {
      unsigned b = 0;
      for (; leafs[b].Empty();) ++b;
      for (unsigned i = b + 1; i < leafs.size(); ++i) {
        if (!leafs[i].Empty() && cmp(leafs[i].s, leafs[b].s)) b = i;
      }
      return leafs[b].GetBestAction(cmp);
    }
  };

 public:
  TNodeInfo s;
  Wait w;
  Seed e;
  Grow g;
  Complete c;
  unsigned k = 0;

  template <class T>
  void Update(const Action& a, const T& x) {
    switch (a.type) {
      case WAIT:
      case AUTO_WAIT:
        w.Update(a, x);
        break;
      case SEED:
        e.Update(a, x);
        break;
      case GROW:
        g.Update(a, x);
        break;
      case COMPLETE:
        c.Update(a, x);
        break;
      default:
        assert(false);
        break;
    }
    s.Update(x);
  }

  template <class TActionIterator>
  void InitS(const Position& p, TActionIterator begin, TActionIterator end) {
    auto F = [&](ActionType t) {
      return std::lower_bound(begin, end, t,
                              [](auto& l, auto& r) { return l.type < r; });
    };
    auto it0 = begin, it1 = F(SEED), it2 = F(GROW), it3 = F(COMPLETE),
         it4 = F(SKIP);
    w.InitS(p, it0, it1);
    e.InitS(p, it1, it2);
    g.InitS(p, it2, it3);
    c.InitS(p, it3, it4);
  }

  void Init(const Position& p, const Actions& va) {
    std::vector<Action> v(va);
    std::sort(v.begin(), v.end());
    InitS(p, v.begin(), v.end());
  }

  Action GetAction() {
    for (++k;; ++k) {
      switch (k % 4) {
        case 0:
          if (!w.Empty()) return w.GetAction();
          break;
        case 1:
          if (!e.Empty()) return e.GetAction();
          break;
        case 2:
          if (!g.Empty()) return g.GetAction();
          break;
        case 3:
          if (!c.Empty()) return c.GetAction();
          break;
      }
    }
  }

  template <class TCompare>
  Action GetBestAction(TCompare cmp) const {
    thread_local std::vector<Node> v;
    v.clear();
    if (!w.Empty()) v.push_back({w.s, w.GetBestAction(cmp)});
    if (!e.Empty()) v.push_back({e.s, e.GetBestAction(cmp)});
    if (!g.Empty()) v.push_back({g.s, g.GetBestAction(cmp)});
    if (!c.Empty()) v.push_back({c.s, c.GetBestAction(cmp)});
    unsigned b = 0;
    for (unsigned i = 1; i < v.size(); ++i) {
      if (cmp(v[i].s, v[b].s)) b = i;
    }
    return v[b].action;
  }
};
