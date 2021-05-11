#pragma once

#include "action.h"
#include "game.h"

class FStrategyWait {
 public:
  static Action Get(const Game&, unsigned) { return Action(WAIT); }
};
