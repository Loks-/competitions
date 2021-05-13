#pragma once

#include "action.h"
#include "evaluation_proxy.h"
#include "game.h"
#include "settings.h"
#include "strategy_time.h"

#include <string>

template <class TFStrategy0, class TFStrategy1>
class StrategyEProxy : public StrategyTime {
 public:
  using TBase = StrategyTime;

 protected:
  EvaluationProxy<TFStrategy0, TFStrategy1> e;
  Game g;

 protected:
  Action FSActionMe() const {
    return Strategy::player ? TFStrategy1::Get(g, 1) : TFStrategy0::Get(g, 0);
  }

  Action FSActionOpp() const {
    return Strategy::player ? TFStrategy0::Get(g, 0) : TFStrategy1::Get(g, 1);
  }

  void Apply(Action a_me, Action a_opp) {
    if (Strategy::player)
      g.ApplyActions(a_opp, a_me);
    else
      g.ApplyActions(a_me, a_opp);
  }

 public:
  void Reset(const Cells& cells) override {
    e.Reset(cells, Strategy::player);
    g.cells = cells;
  }

  std::string Name() const override { return "EProxy"; }

  Action GetAction(const Game& game) override {
    g.pos = game.pos;
    return FSActionMe();
  }

  static PStrategy Make() { return std::make_shared<StrategyEProxy>(); }
};
