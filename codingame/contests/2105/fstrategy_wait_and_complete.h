#pragma once

#include "action.h"
#include "game.h"
#include "settings.h"

class FStrategyWaitAndComplete {
 public:
  static Action Get(const Game& game, unsigned p) {
    if (game.pos.players[p].waiting) return Action(AUTO_WAIT);
    if ((game.pos.day + 1u < TotalDays()) ||
        (game.pos.players[p].sun < CompleteCost()))
      return Action(WAIT);
    for (auto i : game.pos.cell_to_tree) {
      if (i == 255) continue;
      auto& t = game.pos.trees[i];
      if ((t.size != 3) || (t.player != p) || t.used) continue;
      if (CompletePointsFromRichness(game.cells[t.cell].richness) +
              game.pos.nutrients >
          1)
        return Action(COMPLETE, t.cell);
    }
    return Action(WAIT);
  }
};
