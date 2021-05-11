#pragma once

#include "action.h"
#include "game.h"

#include "common/base.h"

class FStrategyRandom {
 public:
  static Action Get(const Game& game, unsigned p) {
    auto m = game.GetPossibleActions(p);
    return m[rand() % m.size()];
  }
};
