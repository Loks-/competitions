#pragma once

#include "action.h"
#include "game.h"
#include "settings.h"

template <unsigned target_trees = 8, unsigned max_seeds = 2>
class FStrategyBuilder {
 public:
  static Action Get(const Game& game, unsigned player) {
    auto& p = game.pos.players[player];
    if (p.waiting) return Action(AUTO_WAIT);
    unsigned days_left = TotalDays() - game.pos.day - 1,
             ntrees = p.ntrees[1] + p.ntrees[2] + p.ntrees[3];
    // Complete
    if ((ntrees >= target_trees) && (p.ntrees[3] > 0) &&
        (p.sun >= CompleteCost())) {
      for (unsigned i = 0; i < CellsSize(); ++i) {
        auto index = game.pos.cell_to_tree[i];
        if (index == 255) continue;
        auto& t = game.pos.trees[index];
        if ((t.player == player) && (t.size == 3) && !t.used)
          return Action(COMPLETE, t.cell);
      }
    }
    // Grow
    if (days_left > 0) {
      for (unsigned tsize = 3; tsize-- > 0;) {
        if ((p.ntrees[tsize] > 0) &&
            (p.sun >= GrowBaseCost(tsize + 1) + p.ntrees[tsize + 1])) {
          for (auto& t : game.pos.trees) {
            if ((t.player == player) && (t.size == tsize) && !t.used)
              return Action(GROW, t.cell);
          }
        }
      }
    }
    // Seed
    if ((p.ntrees[0] < max_seeds) && (p.sun >= SeedBaseCost() + p.ntrees[0])) {
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
