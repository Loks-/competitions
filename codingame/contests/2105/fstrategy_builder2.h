#pragma once

#include "action.h"
#include "game.h"
#include "settings.h"

#include <algorithm>

template <unsigned mt0 = 1, unsigned mt1 = 1, unsigned mt2 = 2,
          unsigned mt3 = 4>
class FStrategyBuilder2 {
 public:
  static Action Get(const Game& game, unsigned player) {
    static const std::vector<unsigned> mt{mt0, mt1, mt2, mt3};
    auto& p = game.pos.players[player];
    if (p.waiting) return Action(AUTO_WAIT);
    unsigned days_left = TotalDays() - game.pos.day - 1;
    // Complete
    if ((p.ntrees[3] >= std::min(days_left + 1, mt3)) &&
        (p.sun >= CompleteCost())) {
      for (unsigned i = 0; i < CellsSize(); ++i) {
        auto index = game.pos.cell_to_tree[i];
        if (index == 255) continue;
        auto& t = game.pos.trees[index];
        if ((t.player == player) && (t.size == 3) && !t.used) {
          if ((game.pos.nutrients > 0) || (days_left == 0)) {
            if (game.pos.nutrients +
                    CompletePointsFromRichness(game.cells[t.cell].richness) >=
                2 + days_left)
              return Action(COMPLETE, t.cell);
          }
          break;
        }
      }
    }
    // Grow
    for (unsigned tsize = 3; tsize-- > 0;) {
      if ((p.ntrees[tsize] > 0) &&
          (p.sun >= GrowBaseCost(tsize + 1) + p.ntrees[tsize + 1]) &&
          (tsize + days_left >= 3) && (p.ntrees[tsize + 1] < mt[tsize + 1])) {
        for (auto& t : game.pos.trees) {
          if ((t.player == player) && (t.size == tsize) && !t.used)
            return Action(GROW, t.cell);
        }
      }
    }
    // Seed
    if ((p.ntrees[0] < mt0) && (p.sun >= SeedBaseCost() + p.ntrees[0]) &&
        ((p.ntrees[0] == 0) || (days_left >= 4))) {
      unsigned best_to = CellsSize(), best_from = CellsSize();
      for (auto& t : game.pos.trees) {
        if ((t.player == player) && !t.used) {
          for (unsigned d = 1; d < t.size; ++d) {
            for (auto cid : game.cells[t.cell].neighbors_by_distance[d]) {
              if (cid >= best_to) continue;
              if ((game.pos.cell_to_tree[cid] == 255) &&
                  (game.cells[cid].richness > 0)) {
                best_to = cid;
                best_from = t.cell;
              }
            }
          }
        }
        if (best_to < CellsSize()) return Action(SEED, best_from, best_to);
      }
    }
    return Action(WAIT);
  }
};
