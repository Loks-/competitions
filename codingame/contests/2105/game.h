#pragma once

#include "action.h"
#include "cell.h"
#include "cells.h"
#include "position.h"
#include "settings.h"

#include "common/base.h"

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
      if ((t.size > 0) && (p.sun >= p.ntrees[0] + SeedBaseCost())) {
        for (uint8_t d = 1; d <= t.size; ++d) {
          for (uint8_t sp : cells[t.cell].neighbors_by_distance[d]) {
            if (cells[sp].richness == 0) continue;
            if (pos.cell_to_tree[sp] != 255) continue;
            r.push_back(Action(SEED, t.cell, sp));
          }
        }
      }
      if ((t.size < 3) &&
          (p.sun >= p.ntrees[t.size + 1] + GrowBaseCost(t.size + 1)))
        r.push_back(Action(GROW, t.cell));
      if ((t.size == 3) && (p.sun >= CompleteCost()))
        r.push_back(Action(COMPLETE, t.cell));
    }
    return r;
  }

  void ApplyAction(unsigned player, const Action& action) {
    auto& p = pos.players[player];
    assert(!p.waiting || (action.type == WAIT));
    switch (action.type) {
      case WAIT: {
        p.waiting = true;
        break;
      }
      case SEED: {
        auto& t = pos.GetTreeByCell(action.value1);
        assert((t.player == player) && !t.used && (t.size > 0) &&
               (pos.cell_to_tree[action.value2] == 255));
        // TODO: check distance
        unsigned dsun = p.ntrees[0] + SeedBaseCost();
        assert(p.sun >= dsun);
        p.sun -= dsun;
        p.ntrees[0] += 1;
        t.used = true;
        pos.AddTree({action.value2, 0, uint8_t(player), true});
        break;
      }
      case GROW: {
        auto& t = pos.GetTreeByCell(action.value1);
        assert((t.player == player) && !t.used && (t.size < 3));
        unsigned dsun = p.ntrees[t.size + 1] + GrowBaseCost(t.size + 1);
        assert(p.sun >= dsun);
        p.sun -= dsun;
        p.ntrees[t.size] -= 1;
        t.size += 1;
        p.ntrees[t.size] += 1;
        t.used = true;
        break;
      }
      case COMPLETE: {
        auto& t = pos.GetTreeByCell(action.value1);
        assert((t.player == player) && !t.used && (t.size == 3));
        unsigned dsun = CompleteCost();
        assert(p.sun >= dsun);
        p.sun -= dsun;
        p.score +=
            pos.nutrients + CompletePointsFromRichness(cells[t.cell].richness);
        p.ntrees[t.size] -= 1;
        pos.DecreaseNutrients();
        pos.RemoveTree(action.value1);
        break;
      }
      default:
        assert(false);
        break;
    }
  }
};
