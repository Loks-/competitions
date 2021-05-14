#pragma once

#include "action.h"
#include "position.h"
#include "runner.h"

#include <utility>
#include <vector>

class Runner2 : public Runner {
 public:
  using TBase = Runner;
  using Action2 = std::pair<Action, Action>;

 protected:
  Position initial;
  std::vector<Action2> actions;

 public:
  void NewGame() {
    TBase::Reset();
    initial = TBase::game.pos;
    actions.clear();
  }

  void RestartGame() {
    TBase::game.pos = initial;
    actions.clear();
  }

  void Apply(const Action2& a) { TBase::game.ApplyActions(a.first, a.second); }

  void RunGame() {
    auto& p0 = TBase::game.pos.players[0];
    auto& p1 = TBase::game.pos.players[1];
    for (; !TBase::game.Ended();) {
      Action a0 =
          p0.waiting ? Action(AUTO_WAIT) : TBase::s0->GetAction(TBase::game);
      Action a1 =
          p1.waiting ? Action(AUTO_WAIT) : TBase::s1->GetAction(TBase::game);
      actions.push_back({a0, a1});
      TBase::game.ApplyActions(a0, a1);
    }
  }

  const std::vector<Action2>& GetActions() const { return actions; }
};
