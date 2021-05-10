#pragma once

#include "action.h"
#include "cell.h"
#include "cells.h"
#include "position.h"
#include "settings.h"

class Game {
 public:
  Cells cells;
  Position pos;

  void ReadCells() { cells.Read(); }
  void ReadPosition() { pos.Read(); }

  Actions GetPossibleActions(unsigned player) const {
    Actions r;
    auto& p = pos.players[player];
    r.push_back(Action(WAIT));
    if (p.waiting) return r;
    for (auto& t : pos.trees) {
      if ((t.player != player) || t.used) continue;
      // SEED
      // ...
      if ((t.size < 3) &&
          (p.sun >= p.ntrees[t.size + 1] + GrowBaseCost(t.size + 1)))
        r.push_back(Action(GROW, t.cell));
      if ((t.size == 3) && (p.sun >= CompleteCost()))
        r.push_back(Action(COMPLETE, t.cell));
    }
    return r;
  }
};
