#pragma once

#include "action.h"
#include "game.h"
#include "strategy.h"

#include "common/base.h"

#include <string>

class StrategyRandomAction : public Strategy {
 public:
  std::string Name() const override { return "Random Action"; }

  Action GetAction(const Game& game) override {
    auto m = game.GetPossibleActions(Strategy::player);
    return m[rand() % m.size()];
  }

  static PStrategy Make() { return std::make_shared<StrategyRandomAction>(); }
};
