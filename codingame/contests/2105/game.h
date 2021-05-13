#pragma once

#include "action.h"
#include "cell.h"
#include "cells.h"
#include "position.h"
#include "settings.h"

#include "common/base.h"

#include <algorithm>

class Game {
 public:
  Cells cells;
  Position pos;

  void ReadCells() { cells.Read(); }
  void ReadPosition() { pos.Read(); }

  bool Ended() const { return pos.day >= TotalDays(); }

  Actions GetPossibleActions(unsigned player) const {
    Actions r;
    auto& p = pos.players[player];
    if (p.waiting) return {Action(AUTO_WAIT)};
    r.push_back(Action(WAIT));
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

  void CalcShades(unsigned day, std::vector<uint8_t>& output) const {
    output.resize(CellsSize());
    std::fill(output.begin(), output.end(), 0);
    unsigned d = day % 6;
    for (auto& t : pos.trees) {
      uint8_t c = t.cell;
      for (unsigned i = 0; i < t.size; ++i) {
        c = cells[c].neighbors[d];
        if (c > CellsSize()) break;
        output[c] = std::max(output[c], t.size);
      }
    }
  }

  void NextDay(bool skip_day_increase) {
    if (!skip_day_increase) pos.day += 1;
    if (pos.day >= TotalDays()) return;
    thread_local std::vector<uint8_t> shade;
    CalcShades(pos.day, shade);
    for (auto& t : pos.trees) {
      if (t.size > shade[t.cell]) pos.players[t.player].sun += t.size;
      t.used = false;
    }
    for (auto& p : pos.players) p.waiting = false;
  }

  int64_t PScore(unsigned p) const {
    return PDScore(pos.players[0].FScore(), pos.players[1].FScore(), p);
  }

  int64_t PScoreExtRaw(unsigned p) const {
    return PDScore(pos.players[0].FScoreExtRaw(), pos.players[1].FScoreExtRaw(),
                   p);
  }

  int64_t PScoreExt(unsigned p) const {
    return UseExtScore() ? PScoreExtRaw(p) : PScore(p);
  }

  void ApplyAction(unsigned player, const Action& action) {
    auto& p = pos.players[player];
    assert(!p.waiting || (action.type == AUTO_WAIT) || (action.type == SKIP));
    switch (action.type) {
      case AUTO_WAIT:
      case WAIT: {
        p.waiting = true;
        break;
      }
      case SEED: {
        auto& t = pos.GetTreeByCell(action.value1);
        assert((t.player == player) && !t.used && (t.size > 0) &&
               (pos.cell_to_tree[action.value2] == 255) &&
               (cells[action.value2].richness > 0));
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
      case SKIP:
        break;
      default:
        assert(false);
        break;
    }
  }

  void ApplyActions(const Action& action0, const Action action1) {
    if ((action0.type == SEED) && (action1.type == SEED) &&
        (action0.value2 == action1.value2)) {
      pos.GetTreeByCell(action0.value1).used = true;
      pos.GetTreeByCell(action1.value1).used = true;
      return;
    }
    if ((action0.type == COMPLETE) && (action1.type == COMPLETE) &&
        (pos.nutrients > 0)) {
      pos.players[1].score += 1;
    }
    ApplyAction(0, action0);
    ApplyAction(1, action1);
    if (pos.players[0].waiting && pos.players[1].waiting) NextDay(false);
  }
};
